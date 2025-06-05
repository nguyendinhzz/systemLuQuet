import json
import os

class DataHandler:
    @staticmethod
    def save_config_data(system_state):
        """Lưu cấu hình của các trạm"""
        try:
            config_data = {
                "water_station": {
                    "warning_level": system_state.water_station["warning_level"]
                },
                "vibration_station": {
                    "threshold": system_state.vibration_station["threshold"]
                }
            }

            # Lấy đường dẫn tuyệt đối của thư mục utils
            base_path = os.path.dirname(os.path.abspath(__file__))
            file_path = os.path.join(base_path, 'data_station.json')
            
            print(f"Lưu file tại: {file_path}")
            
            with open(file_path, 'w', encoding='utf-8') as f:
                json.dump(config_data, f, indent=4, ensure_ascii=False)
            
            print("Đã lưu cấu hình vào data_station.json")
            print(f"Dữ liệu đã lưu: {config_data}")
            return True

        except Exception as e:
            print(f"Lỗi khi lưu cấu hình: {str(e)}")
            return False

    @staticmethod
    def load_config_data():
        """Đọc cấu hình từ file"""
        try:
            # Lấy đường dẫn tuyệt đối của thư mục utils
            base_path = os.path.dirname(os.path.abspath(__file__))
            file_path = os.path.join(base_path, 'data_station.json')
            
            print(f"Đang tìm file tại: {file_path}")
            
            if os.path.exists(file_path):
                print("Tìm thấy file cấu hình")
                with open(file_path, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                    warning_level = data.get('water_station', {}).get('warning_level', "0")
                    vibration_threshold = data.get('vibration_station', {}).get('threshold', "0")
                    print(f"Đọc được warning_level: {warning_level}, vibration_threshold: {vibration_threshold}")
                    return warning_level, vibration_threshold
            else:
                print("Không tìm thấy file cấu hình")
        except Exception as e:
            print(f"Lỗi đọc file cấu hình: {str(e)}")
        
        print("Trả về giá trị mặc định")
        return "0", "0" 