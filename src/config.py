# Cấu hình cơ bản
APP_NAME = "Flood Warning System"
VERSION = "1.0.0"

# Cấu hình Database
DATABASE_FILE = "flood_warning.db"

# Cấu hình UART
UART_PORT = "COM3"  # Thay đổi theo cổng COM mà bạn biết là không bị khóa
UART_BAUDRATE = 9600

# Cấu hình MQTT
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "flood/sensors/#"

# Các ngưỡng cảnh báo
WARNING_LEVELS = {
    "LOW": 30,
    "MEDIUM": 50,
    "HIGH": 70
} 