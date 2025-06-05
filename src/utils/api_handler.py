import requests
import json
from datetime import datetime

class WeatherAPI:
    @staticmethod
    def get_forecast(lat=40.7128, lon=-74.0060, timestamp=False):
        """
        Lấy dữ liệu dự báo từ API
        """
        try:
            url = f"http://127.0.0.1:5000/weather?lat={lat}&lon={lon}"
            response = requests.get(url)

            # Kiểm tra status code trước khi parse JSON
            if response.status_code != 200:
                print(f"Lỗi API: {response.status_code}")
                return None

            # Parse JSON sau khi kiểm tra status code
            data = response.json()
            if not isinstance(data, list):
                print("❌ API không trả về danh sách dữ liệu hợp lệ")
                return None

            # Nếu timestamp=False, trả về dữ liệu như bình thường
            if not timestamp:
                return data
            
            # Lấy thời gian hiện tại
            current_time = datetime.now()

            # Tìm thời gian trước đó gần nhất
            past_forecasts = [
                entry for entry in data
                if 'time' in entry and datetime.strptime(entry['time'], '%Y-%m-%d %H:%M') <= current_time
            ]

            if not past_forecasts:
                print("⚠ Không có dữ liệu dự báo trong quá khứ!")
                return data  # Nếu không có dữ liệu cũ, trả về danh sách gốc

            # Tìm bản ghi có thời gian gần nhất trong quá khứ
            nearest_forecast = max(
                past_forecasts,
                key=lambda x: datetime.strptime(x['time'], '%Y-%m-%d %H:%M')
            )

            # Đưa bản ghi gần nhất lên đầu danh sách
            sorted_data = [nearest_forecast] + [entry for entry in data if entry != nearest_forecast]

            return sorted_data

        except requests.exceptions.RequestException as req_err:
            print(f"Lỗi kết nối: {req_err}")
            return None
        except json.JSONDecodeError:
            print("Lỗi parse JSON từ API")
            return None
        except Exception as e:
            print(f"Lỗi không xác định: {str(e)}")
            return None
