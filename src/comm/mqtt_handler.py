import paho.mqtt.client as mqtt
from ..config import MQTT_BROKER, MQTT_PORT, MQTT_TOPIC

class MQTTHandler:
    def __init__(self):
        self.client = mqtt.Client()
        self.client.on_message = self.on_message
        self.client.on_connect = self.on_connect
        self.connected = False
        
    def connect(self):
        try:
            self.client.connect(MQTT_BROKER, MQTT_PORT, 60)
            return True
        except Exception as e:
            print(f"MQTT Connection Error: {str(e)}")
            return False
            
    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
            self.connected = True
            self.client.subscribe(MQTT_TOPIC)
        else:
            print(f"Failed to connect, return code {rc}")
            
    def on_message(self, client, userdata, message):
        print(f"MQTT Received: {message.payload.decode()}")
        
    def listen(self):
        print("Lopp")
