from dataclasses import dataclass
from PyQt5.QtCore import QObject, pyqtSignal
from time import time
from .data_handler import DataHandler  # Import DataHandler
import sqlite3
from datetime import datetime
from PyQt5.QtCore import QDateTime
from PyQt5.QtChart import QChart, QChartView, QLineSeries, QDateTimeAxis, QValueAxis
from PyQt5.QtGui import QPen, QColor, QPainter
from PyQt5.QtCore import Qt

@dataclass
class SystemState:
    def __init__(self):
        # Load configuration from JSON file
        warning_level, vibration_threshold = DataHandler.load_config_data()
        
        self.water_station = {
            "id": 2,
            "name": "Water Level Station",
            "value": "0",
            "battery": "0",
            "warning_level": warning_level,
            "status": "Offline",
            "last_update": time()
        }
        self.vibration_station = {
            "id": 3,
            "name": "Vibration Station",
            "value": "0",
            "threshold": vibration_threshold,
            "status": "Offline",
            "last_update": time()
        }
        self.is_connected = False
        self.is_running = True  # System is running by default

class GlobalSignals(QObject):
    state_changed = pyqtSignal(SystemState)
    error_occurred = pyqtSignal(str)
    config_saved = pyqtSignal(dict)    # Thêm signal cho việc lưu cấu hình
    connection_status_changed = pyqtSignal(bool)
    water_station_changed = pyqtSignal(dict)    # Signal riêng cho trạm nước
    vibration_station_changed = pyqtSignal(dict)  # Đổi tên

# Khởi tạo các biến toàn cục
system_state = SystemState()
signals = GlobalSignals() 

class DatabaseReader:
    def __init__(self, db_file):
        self.db_file = db_file

    def get_data(self):
        """Lấy dữ liệu từ database"""
        conn = None
        try:
            conn = sqlite3.connect(self.db_file)
            cursor = conn.cursor()
            
            # Truy vấn lấy timestamp và value
            cursor.execute("""
                SELECT timestamp, value 
                FROM water_data 
                ORDER BY timestamp DESC 
                LIMIT 100
            """)
            
            return cursor.fetchall()
            
        except sqlite3.Error as e:
            print(f"Lỗi database: {e}")
            return []
        finally:
            if conn:
                conn.close()

    def update_chart_series(self, series):
        """Cập nhật dữ liệu vào series của biểu đồ"""
        # Lấy dữ liệu từ DB
        data = self.get_data()
        
        # Xóa dữ liệu cũ trong series
        series.clear()
        
        # Thêm dữ liệu mới
        for timestamp_str, value in data:
            # Chuyển timestamp string thành QDateTime
            timestamp = QDateTime.fromString(timestamp_str, "yyyy-MM-dd HH:mm:ss")
            # Thêm điểm dữ liệu vào series
            series.append(timestamp.toMSecsSinceEpoch(), float(value))

class WaterLevelChart:
    def __init__(self, db_path):
        self.db_path = db_path
        self.setup_chart()

    def get_today_data(self):
        """Lấy dữ liệu của ngày hôm nay theo từng giờ"""
        try:
            conn = sqlite3.connect(self.db_path)
            cursor = conn.cursor()
            
            # Lấy dữ liệu từ 00:00:00 đến 23:59:59 của ngày hôm nay
            today = datetime.now().date()
            start_time = datetime.combine(today, time.min)
            end_time = datetime.combine(today, time.max)
            
            query = """
                SELECT timestamp, value 
                FROM water_data 
                WHERE timestamp BETWEEN ? AND ?
                ORDER BY timestamp ASC
            """
            
            cursor.execute(query, (start_time, end_time))
            return cursor.fetchall()
            
        except sqlite3.Error as e:
            print(f"Lỗi database: {e}")
            return []
        finally:
            conn.close()

    def setup_chart(self):
        """Thiết lập biểu đồ"""
        # Tạo series cho dữ liệu
        self.series = QLineSeries()
        self.series.setName("Mực nước")

        # Tạo chart
        self.chart = QChart()
        self.chart.addSeries(self.series)
        self.chart.setTitle("Biểu đồ mực nước theo giờ")
        self.chart.setAnimationOptions(QChart.SeriesAnimations)

        # Thiết lập trục x (thời gian)
        self.axis_x = QDateTimeAxis()
        self.axis_x.setFormat("HH:mm")
        self.axis_x.setTitleText("Thời gian")
        today = datetime.now().date()
        self.axis_x.setRange(
            QDateTime(today.year, today.month, today.day, 0, 0),
            QDateTime(today.year, today.month, today.day, 23, 59)
        )

        # Thiết lập trục y (mực nước)
        self.axis_y = QValueAxis()
        self.axis_y.setRange(0, 250)
        self.axis_y.setTitleText("Mực nước (cm)")
        self.axis_y.setTickCount(11)  # Chia thành 10 khoảng

        # Thêm trục vào chart
        self.chart.addAxis(self.axis_x, Qt.AlignBottom)
        self.chart.addAxis(self.axis_y, Qt.AlignLeft)
        self.series.attachAxis(self.axis_x)
        self.series.attachAxis(self.axis_y)

        # Tạo chart view
        self.chart_view = QChartView(self.chart)
        self.chart_view.setRenderHint(QPainter.RenderHint.Antialiasing)

    def update_chart(self):
        """Cập nhật dữ liệu lên biểu đồ"""
        data = self.get_today_data()
        self.series.clear()

        for timestamp_str, value in data:
            dt = datetime.strptime(timestamp_str, '%Y-%m-%d %H:%M:%S')
            qt_datetime = QDateTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second)
            self.series.append(qt_datetime.toMSecsSinceEpoch(), float(value))

# Sử dụng trong FloodWarningApp
def update_data(self):
    # Khởi tạo DatabaseReader
    db_reader = DatabaseReader("your_database.db")
    # Cập nhật dữ liệu vào series
    db_reader.update_chart_series(self.series) 

class FloodWarningApp:
    def __init__(self):
        # ... existing code ...
        self.water_chart = WaterLevelChart('your_database.db')
        self.layout.addWidget(self.water_chart.chart_view)
        
    def update_display(self):
        """Cập nhật hiển thị"""
        self.water_chart.update_chart() 

class ForecastChart:
    def __init__(self):
        """Khởi tạo biểu đồ dự báo"""
        from .api_handler import WeatherAPI
        self.api = WeatherAPI
        self.setup_chart()

    def setup_chart(self):
        """Thiết lập biểu đồ"""
        # Tạo series cho dữ liệu lũ lụt
        self.flood_series = QLineSeries()
        self.flood_series.setName("flash_flood_probability")
        
        # Tạo series cho dữ liệu sạt lở 
        self.landslide_series = QLineSeries()
        self.landslide_series.setName("landslide_probability")

        # Tạo chart
        self.chart = QChart()
        self.chart.addSeries(self.flood_series)
        self.chart.addSeries(self.landslide_series)
        self.chart.setAnimationOptions(QChart.SeriesAnimations)

        # Thiết lập trục x (thời gian)
        self.axis_x = QDateTimeAxis()
        self.axis_x.setFormat("dd/MM")  # Show date only
        self.axis_x.setTitleText("Date")
        self.axis_x.setTickCount(8)  # Today + 7 days
        
        # Get current date range
        start_date = QDateTime.currentDateTime()
        end_date = start_date.addDays(7)
        self.axis_x.setRange(start_date, end_date)

        # Thiết lập trục y (xác suất)
        self.axis_y = QValueAxis()
        self.axis_y.setRange(0, 100)  # Xác suất từ 0-100%
        self.axis_y.setTitleText("Probability (%)")
        self.axis_y.setTickCount(11)  # Chia thành 10 khoảng
        
        # Tạo pen cho các series
        flood_pen = QPen(QColor("blue"))
        flood_pen.setWidth(2)
        self.flood_series.setPen(flood_pen)
        
        landslide_pen = QPen(QColor("red"))
        landslide_pen.setWidth(2)
        self.landslide_series.setPen(landslide_pen)

        # Thêm trục vào chart
        self.chart.addAxis(self.axis_x, Qt.AlignBottom)
        self.chart.addAxis(self.axis_y, Qt.AlignLeft)
        self.flood_series.attachAxis(self.axis_x)
        self.flood_series.attachAxis(self.axis_y)
        self.landslide_series.attachAxis(self.axis_x)
        self.landslide_series.attachAxis(self.axis_y)

        # Tạo chart view
        self.chart_view = QChartView(self.chart)
        self.chart_view.setRenderHint(QPainter.Antialiasing)

    def update_chart(self):
        """Update chart with forecast data"""
        forecast_data = self.api.get_forecast(timestamp=False)
        
        if not forecast_data:
            return
        
        self.flood_series.clear()
        self.landslide_series.clear()
        
        for data in forecast_data:
            dt = datetime.strptime(data['time'], '%Y-%m-%d %H:%M')
            qt_dt = QDateTime(dt)
            
            self.flood_series.append(qt_dt.toMSecsSinceEpoch(), 
                                   data['flash_flood_probability'])
            self.landslide_series.append(qt_dt.toMSecsSinceEpoch(), 
                                       data['landslide_probability']) 