from sklearn.tree import DecisionTreeClassifier
import numpy as np

class LandslidePredictor:
    def __init__(self):
        # Khởi tạo mô hình cây quyết định
        self.model = DecisionTreeClassifier()
        self.train()
    
    def train(self):
        # Dữ liệu giả lập để huấn luyện mô hình
        X_train = np.array([[10, 20], [30, 40], [50, 60]])
        y_train = np.array([0, 1, 0])
        self.model.fit(X_train, y_train)
    
    def predict(self, features: list[float]):
        # Dự đoán dựa trên đặc trưng đầu vào
        return self.model.predict([features])