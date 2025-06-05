import serial
import struct
import time
from ..config import UART_PORT, UART_BAUDRATE
from ..utils.globals import system_state, signals
from PyQt5.QtWidgets import QMessageBox
from ..utils.database_handler import DatabaseHandler
from ..utils.api_handler import WeatherAPI  # Ensure you import WeatherAPI

class UARTHandler:
    START_BYTE = 0x7E
    TIME_OUT = 16
    TIME_OFFLINE_PRE = -1
    MAX_DATA_SIZE = 32  # Thêm constant cho kích thước tối đa
    MAX_PACKET_SIZE = 37  # START_BYTE + fromID + toID + dataSize + MAX_DATA_SIZE + checksum
    countWarning = 0
    def __init__(self):
        """Khởi tạo UARTHandler"""
        self.port = None
        self.confirm_count = 0
        self.last_config_time = 0
        self.config_timeout = 3  # timeout 3s
        self.db = DatabaseHandler()
        
        # Thêm biến để theo dõi gói tin cảnh báo
        self.alert_count = 0
        self.last_alert_time = 0
        self.alert_timeout = 7  # timeout 7s
        self.first_alert_value = None  # Lưu giá trị lần gửi đầu tiên

    def connect(self):
        """Thiết lập kết nối UART"""
        try:
            # Đóng port nếu đang mở
            if self.port and self.port.is_open:
                self.port.close()
                self.port = None

            # Tạo kết nối mới
            self.port = serial.Serial(
                port=UART_PORT,
                baudrate=UART_BAUDRATE,
                timeout=1,
                bytesize=serial.EIGHTBITS
            )
            print("Kết nối COM thành công")
            # Đợi app hoạt dộng và emit signal 
            time.sleep(0.1)
            signals.connection_status_changed.emit(True)
            return True
        except Exception as e:
            print(f"UART Connection Error: {str(e)}")
            # Emit signal khi kết nối thất bại
            signals.connection_status_changed.emit(False)
            
            # Đảm bảo port được đóng nếu có lỗi
            time.sleep(5)
            if self.port:
                try:
                    self.port.close()
                except:
                    pass
                self.port = None
            return False

    def _check_offline(self):
        """Kiểm tra trạm có bị offline không (quá 30s)"""
        current_time = time.time()
        
        # Kiểm tra trạm nước (ID 2)
        if current_time - system_state.water_station['last_update'] > self.TIME_OUT and system_state.water_station['status'] == "Online" :
            print("update water station offline")
            StateManager.update_state(system_state.water_station, {"status": "Offline"})
            self.TIME_OFFLINE_PRE = 1
            
        # Kiểm tra trạm rung (ID 3)
        if current_time - system_state.vibration_station['last_update'] > self.TIME_OUT and system_state.vibration_station['status'] == "Online":
            print("update vibration station offline")
            StateManager.update_state(system_state.vibration_station, {"status": "Offline"})
        if system_state.water_station['status'] == "Offline" and self.TIME_OFFLINE_PRE > 0 and current_time - self.TIME_OFFLINE_PRE > self.TIME_OUT+2:
            self.TIME_OFFLINE_PRE = current_time
            self.db.insert_log("water_sensor", -1)
    def receive_packet(llself):
        """Nhận và xử lý gói tin hoàn chỉnh"""
        try:
            if not self.port or not self.port.is_open:
                if not self.connect():
                    return None

            # Tìm START_BYTE
            while True:
                if not self.port.in_waiting:
                    return None
                
                start_byte = self.port.read(1)
                if not start_byte:
                    return None
                if start_byte[0] == self.START_BYTE:
                    break
             # Đọc header
            header = self.port.read(3)
            if len(header) != 3:
                return None
            
            fromID, toID, dataSize = struct.unpack('BBB', header)

            # Đọc toàn bộ dữ liệu một lần
            data_bytes = self.port.read(dataSize)
            if len(data_bytes) != dataSize:
                return None
                
            # Chuyển bytes thành list các giá trị
            data = list(data_bytes)

            # Đọc và kiểm tra checksum
            checksum_byte = self.port.read(1)
            if not checksum_byte:
                return None
                
            received_checksum = struct.unpack('B', checksum_byte)[0]  # 8-bit
            calculated_checksum = self._calculate_checksum(fromID, toID, dataSize, data)
            if calculated_checksum != received_checksum:
                print(f"Lỗi: Checksum không khớp! Expected: {calculated_checksum}, Received: {received_checksum}")
                return None

            print("🔄🔄🔄Gói tin nhận thành công:", fromID, toID, dataSize, data)
            return data

        except serial.SerialException as e:
            print(f"Serial Error: {str(e)}")
            self.close()  # Đóng port nếu có lỗi
            return None
        except Exception as e:
            print(f"Lỗi nhận gói tin: {str(e)}")
            return None

    def _handle_uart_packet(self, packet):
        """Process received UART packet"""
        try:
            current_time = time.time()
            
            if packet[0] == 0x54:  # Sensor packet
                # Add to processing queue
                if system_state.water_station['status'] == "Offline":
                    self.TIME_OFFLINE_PRE = -1
                self.db.insert_log("water_sensor", packet[1])
                
                updates = {
                    "value": str(packet[1]),
                    "battery": str(packet[2]),
                    "status": "Online",
                    "last_update": current_time
                }
                StateManager.update_state(system_state.water_station, updates)
            elif packet[0] == 0x57:  # System-wide alert packet
                # Kiểm tra thời gian từ gói tin cảnh báo trước
                if current_time - self.last_alert_time > self.alert_timeout:
                    # Reset counter và giá trị nếu quá thời gian timeout
                    self.alert_count = 0
                    self.first_alert_value = None
                
                # Tăng số đếm và cập nhật thời gian
                self.alert_count += 1
                self.last_alert_time = current_time
                
                # Lưu giá trị lần gửi đầu tiên
                if self.alert_count == 1:
                    self.first_alert_value = packet[1] * 0.7
                # current_flood_prob = WeatherAPI.get_forecast(timestamp=True)
                
                # Chỉ gửi packet khi đủ 2 gói tin trong thời gian quy định và giá trị lần gửi thứ 2 lớn hơn lần gửi đầu tiên
                # if (self.alert_count >= 2 and system_state.is_running and packet[1] > self.first_alert_value) or current_flood_prob[0]['flash_flood_probability'] > 60:
                if (self.alert_count >= 2 and system_state.is_running and packet[1] >= self.first_alert_value) :
                    self.send_packet(from_id=6, to_id=255, data=[0x57])
                    print(f"Forwarded alert packet - Received {self.alert_count} alerts within {self.alert_timeout}s")
                    # Reset counter và giá trị sau khi gửi
                    self.alert_count = 0
                    self.first_alert_value = None  # Reset giá trị
                else:
                    print(f"Alert packet received ({self.alert_count}/2) - Waiting for confirmation")
            elif packet[0] == 0x6C:  # Vibration station packet
                StateManager.update_state(system_state.vibration_station, {
                    "value": str(packet[1]),
                    "status": "Online",
                    "last_update": current_time
                })
            elif packet[0] == 0x36:  # Xóa hoặc cập nhật mã này nếu không còn cần thiết
                StateManager.update_state(system_state.vibration_station, {
                    "value": str(packet[1]),
                    "status": "Online",
                    "last_update": current_time
                })
            elif packet[0] == 0x37:
                print("Canh bao sat lo");
            # Xử lý gói tin config
            elif packet[0] in [0x41, 0x43]:
                self._handle_config_packet(packet[0], packet[1], current_time)

        except Exception as e:
            print(f"Error handling UART packet: {str(e)}")

    def _handle_config_packet(self, packet_type, value, current_time):
        """Xử lý gói tin config và đếm xác nhận"""
        if current_time - self.last_config_time > self.config_timeout:
            self.confirm_count = 0
            
        self.last_config_time = current_time
        self.confirm_count += 2

        if self.confirm_count >= 2:
            if packet_type in [0x41]:  # Water station
                signals.config_saved.emit({"warning_level": value})
                print("Cập nhật cấu hình trạm mực nước thành công!")
            elif packet_type in [0x43]:  # Vibration station
                signals.config_saved.emit({"threshold": value})  # Đổi tên từ on_to_off thành threshold
                print("Cập nhật cấu hình trạm rung thành công!")
            
            time.sleep(1)
            self.confirm_count = 0

    def _calculate_checksum(self, fromID, toID, dataSize, data):
        """Tính toán checksum 8-bit"""
        checksum = fromID + toID + dataSize
        for value in data:
            checksum += value
        return checksum & 0xFF  # Chỉ lấy 8-bit cuối

    def close(self):
        """Đóng kết nối UART và database"""
        if self.port:
            try:
                if self.port.is_open:
                    self.port.close()
            except Exception as e:
                print(f"Error closing port: {str(e)}")
            finally:
                self.port = None
        
        # Đóng kết nối database
        self.db.close()

    def send_packet(self, from_id, to_id, data):
        """
        Gửi gói tin qua UART
        from_id: ID nguồn
        to_id: ID đích
        data: list/bytes chứa dữ liệu cần gửi
        """
        try:
            # Kiểm tra kết nối
            if not self.port or not self.port.is_open:
                print("Không có kết nối UART!")
                return False

            # Kiểm tra kích thước dữ liệu
            data_size = len(data)
            if data_size > self.MAX_DATA_SIZE:
                print("Kích thước dữ liệu vượt quá giới hạn!")
                return False

            # Tạo packet
            packet = bytearray()
            packet.append(self.START_BYTE)  # START_BYTE
            packet.append(from_id)          # From ID
            packet.append(to_id)            # To ID
            packet.append(data_size)        # Data Size

            # Thêm dữ liệu
            packet.extend(data)

            # Tính và thêm checksum
            checksum = self._calculate_checksum(from_id, to_id, data_size, data)
            packet.append(checksum)

            # Gửi packet
            self.port.write(packet)
            print("🔄🔄🔄Gói tin gửi thành công:", from_id, to_id, data_size, data)
            return True

        except Exception as e:
            print(f"Lỗi gửi gói tin: {str(e)}")
            return False

class StateManager:
    @staticmethod
    def update_state(target_dict, updates, emit_signal=True):
        """
        Cập nhật state và emit signal
        target_dict: dictionary cần cập nhật (water_station hoặc vibration_station)
        updates: dict chứa các cặp key-value cần cập nhật
        """
        try:
            # Cập nhật từng key-value
            for key, value in updates.items():
                target_dict[key] = value
            
            # Emit signal tương ứng với trạm
            if emit_signal:
                if target_dict == system_state.water_station:
                    print("Emit water station changed")
                    signals.water_station_changed.emit(target_dict)
                elif target_dict == system_state.vibration_station:
                    print("Emit vibration station changed")
                    signals.vibration_station_changed.emit(target_dict)
            return True
        except Exception as e:
            print(f"Lỗi cập nhật state: {str(e)}")
            return False
