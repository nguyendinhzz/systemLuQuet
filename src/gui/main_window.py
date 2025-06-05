from PyQt5.QtWidgets import (QMainWindow, QLabel, QWidget, QVBoxLayout, QHBoxLayout, 
                            QGridLayout, QFrame, QProgressBar, QGraphicsDropShadowEffect,
                            QPushButton, QLineEdit, QMessageBox)
from PyQt5.QtGui import QPixmap, QFont, QPainter, QColor
from PyQt5.QtCore import Qt, QSize, QDateTime, QTimer
from ..utils.globals import system_state, signals, ForecastChart
from ..core.threads import CommunicationThread
from ..comm.uart_handler import UARTHandler
import os
from pathlib import Path
from .styles import (
    MAIN_STYLE, STATION_CARD_STYLE, GRAPH_FRAME_STYLE, 
    STATUS_FRAME_STYLE, BATTERY_STYLE, BATTERY_TEXT_STYLE, 
    SPEAKER_STATUS_STYLE, SEPARATOR_STYLE, COLORS
)
import time
from ..utils.data_handler import DataHandler
from PyQt5.QtChart import QChart, QChartView, QLineSeries, QDateTimeAxis, QValueAxis
from datetime import datetime, timedelta
import sqlite3
from ..utils.api_handler import WeatherAPI

class StatusIndicator(QLabel):
    def __init__(self, color, size=12, parent=None):
        super().__init__(parent)
        self.confirm_count = 0
        self.setFixedSize(size, size)
        self.color = color

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.setBrush(self.color)
        painter.setPen(Qt.NoPen)
        painter.drawEllipse(0, 0, self.width(), self.height())

class StationCard(QFrame):
    def __init__(self, station_data, uart_handler=None, parent=None):
        super().__init__(parent)
        self.setStyleSheet(STATION_CARD_STYLE)
        self.setGraphicsEffect(self.create_shadow())
        
        self.uart = uart_handler
        self.confirm_count = 0
        
        layout = QVBoxLayout()
        
        # Header with title and status
        header_layout = QHBoxLayout()
        title_label = QLabel(station_data["name"])
        title_label.setFont(QFont("Arial", 14, QFont.Bold))
        status_indicator = StatusIndicator(
            QColor(COLORS["primary"]) if station_data["status"] == "Online" else QColor(COLORS["danger"]),
            size=16
        )
        status_label = QLabel(station_data["status"])
        header_layout.addWidget(title_label)
        header_layout.addStretch()
        header_layout.addWidget(status_indicator)
        header_layout.addWidget(status_label)
        
        # Main value display
        value_layout = QHBoxLayout()
        if "warning_level" in station_data:  # For water station
            value_label = QLabel("Water Level:")
            value_label.setFont(QFont("Arial", 12, QFont.Bold))
            self.value_display = QLabel(f"{station_data['value']} cm")
            self.value_display.setFont(QFont("Arial", 12, QFont.Bold))
            value_layout.addWidget(value_label)
            value_layout.addWidget(self.value_display)
            value_layout.addStretch()
        else:  # For vibration station
            value_label = QLabel("Vibration:")
            value_label.setFont(QFont("Arial", 12, QFont.Bold))
            self.value_display = QLabel(f"{station_data['value']} cm/s²")
            self.value_display.setFont(QFont("Arial", 12, QFont.Bold))
            value_layout.addWidget(value_label)
            value_layout.addWidget(self.value_display)
            value_layout.addStretch()

        # Config layout
        config_layout = QHBoxLayout()
        if "warning_level" in station_data:  # For water station
            warning_label = QLabel("Warning Level:")
            self.warning_input = QLineEdit(station_data["warning_level"])
            warning_unit = QLabel("cm")
        else:  # For vibration station
            warning_label = QLabel("Threshold:")
            self.warning_input = QLineEdit(station_data["threshold"])
            warning_unit = QLabel("cm/s²")

        config_button = QPushButton("Update")
        config_button.clicked.connect(self._on_config_update)
        
        config_layout.addWidget(warning_label)
        config_layout.addWidget(self.warning_input)
        config_layout.addWidget(warning_unit)
        config_layout.addStretch()
        config_layout.addWidget(config_button)
        
        # Battery display
        if "battery" in station_data:
            battery_layout = QVBoxLayout()
            battery_header = QHBoxLayout()

            battery_label = QLabel("Battery:")
            battery_label.setFont(QFont("Arial", 12, QFont.Bold))
            battery_label.setFixedWidth(120)

            battery_level = int(station_data["battery"])
            battery_status = "Low" if battery_level < 15 else "Normal"

            self.battery_display = QLabel(battery_status)
            self.battery_display.setFont(QFont("Arial", 12, QFont.Bold))
            self.battery_display.setStyleSheet("color: red;" if battery_level < 15 else "color: green;")

            battery_header.addWidget(battery_label)
            battery_header.addWidget(self.battery_display)
            
            # Thêm stretch để đẩy các widget về bên trái
            self.battery_progress = QProgressBar()
            self.battery_progress.setValue(battery_level)
            if battery_level >= 15:
                self.battery_progress.setStyleSheet(BATTERY_STYLE["normal"])
            else:
                self.battery_progress.setStyleSheet(BATTERY_STYLE["low"])

            battery_layout.addLayout(battery_header)
            battery_layout.addWidget(self.battery_progress)

        layout.addLayout(header_layout)
        layout.addLayout(value_layout)
        layout.addLayout(config_layout)
        if "battery" in station_data:
            layout.addLayout(battery_layout)

        self.setLayout(layout)


        # Save references for updates
        self.status_indicator = status_indicator
        self.status_label = status_label
        self.is_water_station = "warning_level" in station_data

    def _on_config_update(self):
        """Handle configuration update button click"""
        try:
            new_value = self.warning_input.text()
            
            # Check if station is online
            if self.is_water_station and system_state.water_station["status"] == "Offline":
                QMessageBox.warning(self, "Error", "Water station is offline!")
                self.warning_input.setText(self.original_value)
                return
            elif not self.is_water_station and system_state.speaker_station["status"] == "Offline":
                QMessageBox.warning(self, "Error", "Speaker station is offline!")
                self.warning_input.setText(self.original_value)
                return

            # Check if value has changed
            if new_value == self.original_value:
                QMessageBox.warning(self, "Warning", "Please enter a new value!")
                return

            # Validate input
            if self.is_water_station:
                try:
                    float_val = float(new_value)
                    if float_val <= 0:
                        raise ValueError("Value must be greater than 0")
                except ValueError:
                    QMessageBox.warning(self, "Error", "Please enter a valid positive number")
                    self.warning_input.setText(self.original_value)
                    return
            else:
                try:
                    int_val = int(new_value)
                    if int_val <= 0:
                        raise ValueError("Value must be greater than 0")
                except ValueError:
                    QMessageBox.warning(self, "Error", "Please enter a valid positive integer")
                    self.warning_input.setText(self.original_value)
                    return

            # Show confirmation dialog
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Icon.Question)
            msg.setWindowTitle("Confirm")
            
            if self.is_water_station:
                msg.setText(f"Are you sure you want to change the warning level to {new_value}m?")
            else:
                msg.setText(f"Are you sure you want to change the auto-off time to {new_value}s?")
            
            msg.setStandardButtons(QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No)
            msg.setDefaultButton(QMessageBox.StandardButton.No)

            if msg.exec() == QMessageBox.StandardButton.Yes:
                # Reset confirm count in UART handler
                self.uart.confirm_count = 0
                self.uart.last_config_time = time.time()
                
                # Send packet
                if not self.uart.send_packet(from_id=6, to_id=2, data=[0x42, int(float_val)]):
                    QMessageBox.warning(self, "Error", "Cannot send packet! (Error 00)")
                    return

                # Wait for response (3 seconds)
                start_time = time.time()
                while time.time() - start_time < 3:
                    if self.uart.confirm_count >= 2:
                        # Update system_state before
                        if self.is_water_station:
                            system_state.water_station["warning_level"] = str(new_value)
                        else:
                            system_state.speaker_station["on_to_off"] = str(new_value)

                        # Then save to file
                        DataHandler.save_config_data(system_state)
                        
                        # Update UI
                        self.original_value = new_value
                        QMessageBox.information(self, "Success", "Configuration update successful!")
                        return
                    time.sleep(0.1)

                # Show error message if timeout
                if self.uart.confirm_count == 0:
                    QMessageBox.warning(self, "Error", "Device did not respond")
                elif self.uart.confirm_count == 1:
                    QMessageBox.warning(self, "Error", "Device did not respond")

        except Exception as e:
            print(f"Error: {str(e)}")
            self.warning_input.setText(str(self.original_value))

    def create_shadow(self):
        """Tạo hiệu ứng đổ bóng cho card"""
        shadow = QGraphicsDropShadowEffect()
        shadow.setBlurRadius(15)
        shadow.setOffset(0, 0)
        shadow.setColor(QColor(0, 0, 0, 60))
        return shadow

class SystemState:
    def __init__(self):
        # Load configuration from JSON file
        warning_level, threshold = DataHandler.load_config_data()
        
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
            "threshold": threshold,
            "status": "Offline",
            "last_update": time()
        }
        self.is_connected = False

class UIStationUpdater:
    def __init__(self, main_window):
        self.main_window = main_window

    def update_water_station(self, state):
        """Update water station status"""
        try:
            # Check if state has data
            if not state:
                print("No state data")
                return
            
            # Check connection status
            if 'status' in state:
                if state['status'] == "Offline":
                    self.main_window.water_station_card.status_label.setText("Offline")
                    self.main_window.water_station_card.status_indicator.color = QColor(COLORS["danger"])
                    self.main_window.water_station_card.status_indicator.update()
                else:
                    self.main_window.water_station_card.status_label.setText("Online")
                    self.main_window.water_station_card.status_indicator.color = QColor(COLORS["primary"])
                    self.main_window.water_station_card.status_indicator.update()
                # Update battery
            
           
            battery_level = int(state['battery'])
            self.main_window.water_station_card.battery_display.setText("Low" if battery_level < 15 else "Normal")

            # Update battery bar and style
            if hasattr(self.main_window.water_station_card, 'battery_progress'):
                self.main_window.water_station_card.battery_progress.setValue(battery_level)
                if battery_level >= 15:
                    self.main_window.water_station_card.battery_progress.setStyleSheet(BATTERY_STYLE["normal"])
                    self.main_window.water_station_card.battery_display.setStyleSheet(BATTERY_TEXT_STYLE["normal"])
                else:
                    self.main_window.water_station_card.battery_progress.setStyleSheet(BATTERY_STYLE["low"])
                    self.main_window.water_station_card.battery_display.setStyleSheet(BATTERY_TEXT_STYLE["low"])
            # Update water level value if available
            if 'value' in state:
                self.main_window.water_station_card.value_display.setText(f"{state['value']} cm")
                
                # Update data in chart
                # current_time = QDateTime.currentDateTime()
                # self.main_window.series.append(current_time.toMSecsSinceEpoch(), float(state['value']))
                
                # # Update x-axis range
                # self.main_window.axis_x.setRange(
                #     current_time.addSecs(-3 * 3600),
                #     current_time
                # )

        except Exception as e:
            print(f"Error updating water station: {e}")

    def update_vibration_station(self, state):
        """Update UI for vibration station"""
        try:
            if not hasattr(self.main_window, 'vibration_station_card'):
                return

            card = self.main_window.vibration_station_card
            
            # Kiểm tra xem state có phải là dict và có chứa các khóa cần thiết không
            if not isinstance(state, dict) or 'status' not in state:
                print("Invalid state data for vibration station")
                return
            
            if state['status'] == "Offline":
                print("update GUI vibration")
                card.status_label.setText("Offline")
                card.status_indicator.color = QColor(COLORS["danger"])
                card.status_indicator.update()
                return

            # Update online status
            card.status_label.setText("Online")
            card.status_indicator.color = QColor(COLORS["primary"])
            card.status_indicator.update()

            # Update threshold if value exists
            if 'value' in state:
                card.value_display.setText(f"{state['value']} cm/s²")
                card.value_display.setStyleSheet(
                    SPEAKER_STATUS_STYLE["on"]
                )
                
        except Exception as e:
            print(f"Error updating vibration station UI: {str(e)}")

class FloodWarningApp(QMainWindow):
    def __init__(self):
        super().__init__()
        
        # Initialize forecast chart first
        self.forecast_chart = ForecastChart()
        
        # Then initialize UI components
        self.ui_updater = UIStationUpdater(self)
        self._init_components()
        self._init_ui()
        self._setup_connections()
        
        # Update chart
        self.update_forecast_data()
        
        # Create timer for scheduled updates
        self.update_timer = QTimer()
        self.update_timer.timeout.connect(self.update_forecast_data)
        self.update_timer.start(24 * 60 * 60 * 1000)  # 24 hours
        
        # Initialize database
        self.init_database()
        
        # Add sample data for testing
        self.insert_sample_data()
        
        # Khởi tạo biến toàn cục cho xác suất
        self.current_flood_prob = 0
        
        # Tạo timer để cập nhật xác suất mỗi khung giờ chia hết cho 3
        self.update_timer = QTimer()
        self.update_timer.timeout.connect(self.update_flood_probability)
        self.update_timer.start(30 *60 * 1000)  # Cập nhật mỗi gio

    def _init_components(self):
        """Initialize processing components"""
        self.comm_thread = CommunicationThread()
        # Save reference to uart handler
        self.uart = self.comm_thread.uart
        self.comm_thread.start()

    def _init_ui(self):
        """Initialize user interface"""
        self.setWindowTitle("Flood Warning Application")
        self.setGeometry(100, 100, 1920, 1080)
        
        # Central widget
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        central_widget.setStyleSheet(MAIN_STYLE)
        
        # Main layout
        main_layout = QVBoxLayout(central_widget)
        main_layout.setContentsMargins(20, 20, 20, 20)
        main_layout.setSpacing(20)

        # Add top container
        top_container = self.create_top_status_bar()
        main_layout.addLayout(top_container)

        # Content area
        content_layout = QHBoxLayout()
        content_layout.setSpacing(20)

        # Left side - Graph
        graph_frame = QFrame()
        graph_frame.setStyleSheet(GRAPH_FRAME_STYLE)
        graph_frame.setGraphicsEffect(self.create_shadow())
        graph_layout = QVBoxLayout(graph_frame)
        
        # Chart title
        chart_title = QLabel("7-Day Flood and Landslide Probability Forecast")
        chart_title.setAlignment(Qt.AlignCenter)
        chart_title.setFont(QFont("Arial", 16, QFont.Bold))
        
        # Add chart view to layout
        graph_layout.addWidget(chart_title)
        graph_layout.addWidget(self.forecast_chart.chart_view)
        
        # Add graph frame to content layout
        content_layout.addWidget(graph_frame, 70)
        
        # Right side - Station cards
        stations_layout = QVBoxLayout()
        stations_layout.setSpacing(15)

        # Water station
        self.water_station_card = StationCard(system_state.water_station, uart_handler=self.uart)
        stations_layout.addWidget(self.water_station_card)

        # Spacing between stations
        spacer = QWidget()
        spacer.setFixedHeight(20)
        stations_layout.addWidget(spacer)

        # Vibration station
        self.vibration_station_card = StationCard(system_state.vibration_station, uart_handler=self.uart)
        stations_layout.addWidget(self.vibration_station_card)

        # Add empty space at the end
        stations_layout.addStretch()

        # Add stations_layout to content_layout
        content_layout.addLayout(stations_layout, stretch=40)

        main_layout.addLayout(content_layout)

        # Thiết lập hệ thống mặc định là đang chạy
        system_state.is_running = True
        self.system_status_label.setText("System: Running")
        self.system_status_indicator.color = QColor(COLORS["primary"])
        self.system_status_indicator.update()

    def create_top_status_bar(self):
        top_container = QVBoxLayout()
        top_container.setSpacing(10)
        
        # Status indicators
        status_row = QHBoxLayout()
        status_row.setContentsMargins(0, 0, 0, 0)
        
        # Left frame for probabilities
        left_status_frame = QFrame()
        left_status_frame.setFixedHeight(40)
        left_status_frame.setStyleSheet(STATUS_FRAME_STYLE)
        left_status_frame.setGraphicsEffect(self.create_shadow())
        
        left_status_layout = QHBoxLayout(left_status_frame)
        left_status_layout.setContentsMargins(10, 0, 10, 0)
        left_status_layout.setSpacing(10)
        
        # Flood probability
        flood_layout = QHBoxLayout()
        self.flood_prob_label = QLabel("-- % flood probability")
        self.flood_prob_label.setFont(QFont("Arial", 12))
        self.flood_indicator = StatusIndicator(QColor(COLORS["primary"]), size=8)
        flood_layout.addWidget(self.flood_indicator)
        flood_layout.addWidget(self.flood_prob_label)
        
        # Separator
        separator1 = QFrame()
        separator1.setFrameShape(QFrame.VLine)
        separator1.setStyleSheet(SEPARATOR_STYLE)
        
        # Landslide probability
        landslide_layout = QHBoxLayout()
        self.landslide_prob_label = QLabel("-- % landslide probability")
        self.landslide_prob_label.setFont(QFont("Arial", 12))
        self.landslide_indicator = StatusIndicator(QColor(COLORS["primary"]), size=8)
        landslide_layout.addWidget(self.landslide_indicator)
        landslide_layout.addWidget(self.landslide_prob_label)
        
        # Add layouts to left frame
        left_status_layout.addLayout(flood_layout)
        left_status_layout.addWidget(separator1)
        left_status_layout.addLayout(landslide_layout)
        
        # Right frame for system and connection status
        right_status_frame = QFrame()
        right_status_frame.setFixedHeight(40)
        right_status_frame.setStyleSheet(STATUS_FRAME_STYLE)
        right_status_frame.setGraphicsEffect(self.create_shadow())
        
        right_status_layout = QHBoxLayout(right_status_frame)
        right_status_layout.setContentsMargins(10, 0, 10, 0)
        right_status_layout.setSpacing(10)
        
        # System status
        system_layout = QHBoxLayout()
        self.system_status_label = QLabel("System: Running")
        self.system_status_label.setFont(QFont("Arial", 12))
        self.system_status_indicator = StatusIndicator(QColor(COLORS["primary"]), size=8)
        system_layout.addWidget(self.system_status_indicator)
        system_layout.addWidget(self.system_status_label)
        
        # System switch button
        self.system_switch = QPushButton("Stop System")
        self.system_switch.setCheckable(True)
        self.system_switch.setFixedWidth(125)
        self.system_switch.setChecked(True)
        self.system_switch.clicked.connect(self.toggle_system_status)
        self.system_switch.setStyleSheet("""
            QPushButton {
                background-color: #2ecc71;
                color: white;
                border: none;
                border-radius: 4px;
                padding: 4px 8px;
            }
            QPushButton:checked {
                background-color: #e74c3c;
            }
            QPushButton:disabled {
                background-color: #7f8c8d;
                color: #ecf0f1;
            }
        """)
        
        # Disable switch if not connected
        self.system_switch.setEnabled(system_state.is_connected)
        
        system_layout.addWidget(self.system_switch)
        
        # Separator
        separator_sys = QFrame()
        separator_sys.setFrameShape(QFrame.VLine)
        separator_sys.setStyleSheet(SEPARATOR_STYLE)
        
        # Connection status
        connection_layout = QHBoxLayout()
        self.connection_status = QLabel("Port: Connected")
        self.connection_status.setFont(QFont("Arial", 12))
        self.connection_indicator = StatusIndicator(QColor(COLORS["primary"]), size=8)
        connection_layout.addWidget(self.connection_indicator)
        connection_layout.addWidget(self.connection_status)
        
        # Add layouts to right frame
        right_status_layout.addLayout(system_layout)
        right_status_layout.addWidget(separator_sys)
        right_status_layout.addLayout(connection_layout)
        
        # Add frames to status row with properly ordered positioning
        status_row.addWidget(left_status_frame)
        status_row.addStretch()
        status_row.addWidget(right_status_frame)
        
        # Header
        header_row = QHBoxLayout()
        title = QLabel("Flood Warning Prediction")
        title.setFont(QFont("Arial", 24, QFont.Bold))
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet("color: #2c3e50;")

        header_row.addStretch()
        header_row.addWidget(title)
        header_row.addStretch()

        top_container.addLayout(status_row)
        top_container.addLayout(header_row)
        
        return top_container


    def create_shadow(self):
        shadow = QGraphicsDropShadowEffect()
        shadow.setBlurRadius(15)
        shadow.setOffset(0, 0)
        shadow.setColor(QColor(0, 0, 0, 60))
        return shadow

    def _setup_connections(self):
        """Thiết lập các kết nối và signals"""
        signals.water_station_changed.connect(self.ui_updater.update_water_station)
        signals.vibration_station_changed.connect(self.ui_updater.update_vibration_station)
        signals.error_occurred.connect(self._handle_error)
        signals.connection_status_changed.connect(self._update_connection_status)

    def _handle_error(self, error_msg):
        """Xử lý lỗi từ các luồng"""
        print(f"Error: {error_msg}")
        print(f"Error: {error_msg}")
        print(f"Error: {error_msg}")
        """Xử lý lỗi từ các luồng"""
        print(f"Error: {error_msg}")
        print(f"Error: {error_msg}")
        """Xử lý lỗi từ các luồng"""
        print(f"Error: {error_msg}")

    def _calculate_checksum(self, fromID, toID, title, data):
        """Tính toán checksum cho gói tin tương thích với chip xử lý"""
        checksum = fromID + toID + title
        for value in data:
            checksum += value
        return checksum & 0xFF  # Chỉ lấy 8 bit thấp nhất

    def _update_connection_status(self, is_connected):
        """Update connection status on the UI"""
        try:
            if is_connected:
                self.connection_status.setText("Port: Connected")
                self.connection_indicator.color = QColor(COLORS["primary"])
                system_state.is_connected = True
                # Enable system switch when connected
                self.system_switch.setEnabled(True)
                
                # Auto-start system when connected
                system_state.is_running = True
                self.system_switch.setChecked(True)
                self.system_status_label.setText("System: Running")
                self.system_status_indicator.color = QColor(COLORS["primary"])
                self.system_switch.setText("Stop System")
            else:
                self.connection_status.setText("Port: Disconnected")
                self.connection_indicator.color = QColor(COLORS["danger"])
                # system_state.is_connected = False
                
                # # When disconnected, show system as paused but keep state
                # self.system_status_label.setText("System: Waiting for connection")
                # self.system_status_indicator.color = QColor(COLORS["danger"])
                # self.system_status_indicator.update()
                
                # # Disable system switch when disconnected
                # self.system_switch.setEnabled(False)
            
            self.connection_indicator.update()
            
        except Exception as e:
            print(f"Error updating connection status: {str(e)}")

    def update_forecast_data(self):
        """Update forecast data and chart"""
        self.forecast_chart.update_chart()
        
        # Update current flood and landslide probability indicators in status bar
        # Get the latest probability value (first in the forecast list)
        current_time = datetime.now()
        timestamp = current_time.strftime('%Y-%m-%d %H:%M:%S')  # Định dạng thời gian
        forecast_data = WeatherAPI.get_forecast(timestamp=True)
        
        if forecast_data and len(forecast_data) > 0:
            current_flood_prob = forecast_data[0]['flash_flood_probability']
            current_landslide_prob = forecast_data[0]['landslide_probability']
            
            # Update UI
            self.update_status_bar(current_flood_prob, current_landslide_prob)
    
    def update_status_bar(self, flood_prob, landslide_prob):
        """Update status bar with latest probabilities"""
        # Determine colors for indicators
        flood_color = self.get_risk_color(flood_prob)
        landslide_color = self.get_risk_color(landslide_prob)
        
        # Update UI
        if hasattr(self, 'flood_indicator') and hasattr(self, 'landslide_indicator'):
            self.flood_indicator.color = flood_color
            self.flood_prob_label.setText(f"{flood_prob:.1f}% flood probability")
            
            self.landslide_indicator.color = landslide_color
            self.landslide_prob_label.setText(f"{landslide_prob:.1f}% landslide probability")
            
            # Force UI update
            self.flood_indicator.update()
            self.landslide_indicator.update()
    
    def get_risk_color(self, probability):
        """Return color based on risk level"""
        if probability < 30:
            return QColor(COLORS["primary"])  # Green - Safe
        elif probability < 70:
            return QColor("#FFA500")  # Orange - Warning
        else:
            return QColor(COLORS["danger"])  # Red - Danger

    def closeEvent(self, event):
        """Handle window close event"""
        try:
            # Stop thread and close connections
            if hasattr(self, 'comm_thread'):
                self.comm_thread.stop()
            
            # Close window
            event.accept()
            print("Application closed and port released")
        except Exception as e:
            print(f"Error while closing application: {str(e)}")
            event.accept()  # Still close application even with errors

    def init_database(self):
        """Initialize database and create tables if not exist"""
        try:
            conn = sqlite3.connect('flood_warning.db')  # or your database name
            cursor = conn.cursor()
            
            # Create table if it doesn't exist
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS water_data (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    timestamp DATETIME NOT NULL,
                    value FLOAT NOT NULL
                )
            """)
            
            conn.commit()
        except sqlite3.Error as e:
            print(f"Database initialization error: {e}")
        finally:
            conn.close()

    def insert_sample_data(self):
        """Add sample data for testing"""
        try:
            conn = sqlite3.connect('flood_warning.db')
            cursor = conn.cursor()
            
            # Delete old data
            cursor.execute("DELETE FROM water_data")
            
            # Add sample data for 3 hours
            current_time = datetime.now()
            for hour in range(3):
                timestamp = current_time - timedelta(hours=3-hour)
                # Create sample values ranging from 100-150cm
                value = 100 + (hour % 5) * 10  
                
                cursor.execute("""
                    INSERT INTO water_data (timestamp, value)
                    VALUES (?, ?)
                """, (timestamp.strftime('%Y-%m-%d %H:%M:%S'), value))
            
            conn.commit()
        except sqlite3.Error as e:
            print(f"Error adding sample data: {e}")
        finally:
            conn.close()

    def _update_ui(self, state):
        """Update UI when state changes"""
        try:
            self.ui_updater.update_water_station(state)
            self.ui_updater.update_vibration_station(state)
            print("UI updated with new values")
        except Exception as e:
            print(f"Error updating UI: {str(e)}")
            import traceback
            print(traceback.format_exc())

    # Add a new method to update the system status
    def update_system_status(self, status, is_ok=True):
        """Update system status label and indicator"""
        try:
            self.system_status_label.setText(f"System: {status}")
            if is_ok:
                self.system_status_indicator.color = QColor(COLORS["primary"])
            else:
                self.system_status_indicator.color = QColor(COLORS["danger"])
            
            self.system_status_indicator.update()
        except Exception as e:
            print(f"Error updating system status: {str(e)}")

    def toggle_system_status(self):
        """Toggle the system running state"""
        try:
            if self.system_switch.isChecked():
                # System is running
                system_state.is_running = True
                self.system_status_label.setText("System: Running")
                self.system_status_indicator.color = QColor(COLORS["primary"])
                self.system_switch.setText("Stop System")
                print("System resumed")
            else:
                # System is paused/stopped
                system_state.is_running = False
                self.system_status_label.setText("System: Paused")
                self.system_status_indicator.color = QColor(COLORS["danger"])
                self.system_switch.setText("Resume System")
                print("System paused")
                
                # Send stop packet (0x58) when system is paused
                if system_state.is_connected:
                    self.uart.send_packet(from_id=6, to_id=255, data=[0x58])
                    print("System pause packet sent (0x58)")
            
            # Update the indicator
            self.system_status_indicator.update()
            
        except Exception as e:
            print(f"Error toggling system status: {str(e)}")

    def update_flood_probability(self):
        """Cập nhật xác suất lũ lụt"""
            
        forecast_data = WeatherAPI.get_forecast(timestamp=True)
        if forecast_data:
            self.current_flood_prob = forecast_data[0]['flash_flood_probability']
            self.current_landslide_prob = forecast_data[0]['landslide_probability']
            print(f"Cập nhật xác suất lũ lụt: {self.current_flood_prob}%")
            
            # Cập nhật giao diện nếu cần
            self.update_status_bar(self.current_flood_prob, self.current_landslide_prob)
