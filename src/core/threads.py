import threading
from typing import Optional
from ..utils.globals import system_state, signals
from ..comm.uart_handler import UARTHandler
from ..comm.mqtt_handler import MQTTHandler
import time

class CommunicationThread:
    def __init__(self):
        self.uart = UARTHandler()
        self.mqtt = MQTTHandler()
        self._uart_thread: Optional[threading.Thread] = None
        self._mqtt_thread: Optional[threading.Thread] = None
        self._running = False

    def start(self):
        try:
            self._running = True
            self._uart_thread = threading.Thread(
                target=self._uart_listen_loop, 
                daemon=True
            )
            
            self._mqtt_thread = threading.Thread(
                target=self.mqtt.listen,
                daemon=True
            )
            
            self._uart_thread.start()
            self._mqtt_thread.start()
            
            system_state.is_connected = True
            signals.state_changed.emit(system_state)
            print("Communication threads started.")
            
        except Exception as e:
            signals.error_occurred.emit(f"Thread error: {str(e)}")
            print(f"Error starting threads: {str(e)}")
    
    def _uart_listen_loop(self):
        """Vòng lặp liên tục lắng nghe UART"""
        while self._running:
            try:
                packet = self.uart.receive_packet()
                if packet:
                    self.uart._handle_uart_packet(packet)
                time.sleep(0.1)  # Tránh tiêu tốn CPU
                self.uart._check_offline()
            except Exception as e:
                print(f"UART listening error: {str(e)}")
                time.sleep(1)  # Đợi 1 giây trước khi thử lại
    
    
            
    def stop(self):
        """Dừng các luồng và đóng kết nối"""
        self._running = False
        if self.uart:
            self.uart.close()
        system_state.is_connected = False
        signals.state_changed.emit(system_state)
        print("Communication threads stopped.") 