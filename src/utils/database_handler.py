import sqlite3
from datetime import datetime
import os
from queue import Queue, Empty
import threading
import time

class DatabaseHandler:
    def __init__(self):
        self.queue = Queue()
        self.running = True
        self.conn = None
        self.db_thread = threading.Thread(target=self._process_queue, daemon=True)
        self.db_thread.start()
        print("Đã khởi tạo DatabaseHandler")

    def _init_db(self):
        """Khởi tạo kết nối database"""
        try:
            base_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            data_path = os.path.join(base_path, 'data')
            
            if not os.path.exists(data_path):
                os.makedirs(data_path)
            
            current_month = datetime.now().strftime("%Y_%m")
            db_file = os.path.join(data_path, f'sensor_logs_{current_month}.db')
            print(f"Tạo kết nối DB tại: {db_file}")
            
            conn = sqlite3.connect(db_file)
            cursor = conn.cursor()
            cursor.execute('''
                CREATE TABLE IF NOT EXISTS sensor_logs (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    timestamp DATETIME NOT NULL,
                    sensor_name TEXT NOT NULL,
                    value REAL NOT NULL
                )
            ''')
            conn.commit()
            print("Đã tạo bảng sensor_logs")
            return conn
        except Exception as e:
            print(f"Lỗi khởi tạo database: {str(e)}")
            return None

    def _process_queue(self):
        """Xử lý queue trong một thread riêng"""
        print("Bắt đầu thread xử lý queue")
        retry_count = 0
        max_retries = 3

        while self.running:
            try:
                if self.conn is None:
                    self.conn = self._init_db()
                    if self.conn is None:
                        print("Không thể kết nối database, thử lại sau 5s")
                        time.sleep(5)
                        continue

                # Đợi item trong queue với timeout 1s
                try:
                    item = self.queue.get(timeout=1)
                except Empty:
                    continue

                if item is None:
                    continue

                sensor_name, value = item
                cursor = self.conn.cursor()
                cursor.execute('''
                    INSERT INTO sensor_logs (timestamp, sensor_name, value)
                    VALUES (?, ?, ?)
                ''', (datetime.now(), sensor_name, value))
                self.conn.commit()
                self.queue.task_done()
                print(f"Đã lưu dữ liệu: {sensor_name} = {value}")
                retry_count = 0  # Reset số lần thử lại khi thành công

            except sqlite3.Error as e:
                print(f"Lỗi SQLite: {str(e)}")
                retry_count += 1
                if retry_count >= max_retries:
                    print("Đã thử lại quá số lần cho phép, đóng kết nối")
                    if self.conn:
                        self.conn.close()
                    self.conn = None
                    retry_count = 0
                time.sleep(1)

            except Exception as e:
                print(f"Lỗi xử lý queue: {str(e)}")
                time.sleep(1)

        # Đóng kết nối khi dừng
        if self.conn:
            try:
                self.conn.close()
                print("Đã đóng kết nối database")
            except Exception as e:
                print(f"Lỗi đóng kết nối: {str(e)}")

    def insert_log(self, sensor_name, value):
        """Thêm dữ liệu vào queue"""
        try:
            if not self.running:
                print("DatabaseHandler đã dừng")
                return False

            self.queue.put((sensor_name, value))
            # print(f"Đã thêm vào queue: {sensor_name} = {value}")
            return True
        except Exception as e:
            print(f"Lỗi thêm vào queue: {str(e)}")
            return False

    def get_logs(self, sensor_name=None, start_date=None, end_date=None):
        """Lấy dữ liệu theo điều kiện"""
        try:
            if not self.conn:
                return []

            cursor = self.conn.cursor()
            query = "SELECT * FROM sensor_logs WHERE 1=1"
            params = []

            if sensor_name:
                query += " AND sensor_name = ?"
                params.append(sensor_name)

            if start_date:
                query += " AND timestamp >= ?"
                params.append(start_date)

            if end_date:
                query += " AND timestamp <= ?"
                params.append(end_date)

            cursor.execute(query, params)
            return cursor.fetchall()
        except Exception as e:
            print(f"Lỗi truy vấn dữ liệu: {str(e)}")
            return []

    def close(self):
        """Dừng thread xử lý và đóng kết nối"""
        try:
            print("Đang dừng DatabaseHandler...")
            # self.running = False
            
            # Đợi queue xử lý hết dữ liệu
            if self.queue.qsize() > 0:
                print(f"Đợi xử lý {self.queue.qsize()} items còn lại...")
                self.queue.join()
            
            if self.db_thread.is_alive():
                self.db_thread.join(timeout=5)
                print("Đã dừng thread xử lý")
            
        except Exception as e:
            print(f"Lỗi khi đóng DatabaseHandler: {str(e)}")