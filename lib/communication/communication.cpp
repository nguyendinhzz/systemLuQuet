#include "communication.h"

uint8_t loraBuf[RH_MESH_MAX_MESSAGE_LEN];
const char *mqtt_server = "192.168.100.123";
const int mqtt_port = 1883;
const char *mqtt_topic = "ping";
String tokenAccess;

WiFiClient espClient;
PubSubClient client(espClient);
void reconnectMQTT() {
    if (connectWifi&&!client.connected()) {
            Serial.print("Attempting MQTT connection...");
            if (client.connect("ESP32Client")) {
            Serial.println("connected");
            } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            // Serial.println(" try again in 5 seconds");
            // delay(5000);
            }
        
    }
    // client.loop();
}
void setupMQTT(){
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}
void mqttSend(String jsonStr){
    // Gửi dữ liệu tới topic
    client.publish(mqtt_topic, jsonStr.c_str());
}
void callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (String(topic) == "/data") {
    Serial.println("Processing sensor data...");
  }
}

void packageToBuffer(const packageData &package, uint8_t *buffer, size_t &length) {
    buffer[0] = package.title.length();
    memcpy(&buffer[1], package.title.c_str(), package.title.length());
    uint8_t dataStartIndex = 1 + package.title.length();
    for (size_t i = 0; i < package.data.size(); ++i) {
        float val = package.data[i];
        memcpy(&buffer[dataStartIndex + i * sizeof(float)], &val, sizeof(float));
    }
    length = dataStartIndex + package.data.size() * sizeof(float);
}

void bufferToPackage(const uint8_t *buffer, size_t length, packageData &package) {
    uint8_t titleLength = buffer[0];
    char titleBuffer[titleLength + 1]; 
    memcpy(titleBuffer, &buffer[1], titleLength);
    titleBuffer[titleLength] = '\0';
    package.title = String(titleBuffer);

    uint8_t dataStartIndex = 1 + titleLength;
    package.data.clear();
    for (size_t i = dataStartIndex; i < length; i += sizeof(float)) {
        float val;
        memcpy(&val, &buffer[i], sizeof(float));
        package.data.push_back(val);
    }
}

int sendData(const packageData &package, int nodeIdCenter)
{
    uint8_t buffer[RH_MESH_MAX_MESSAGE_LEN];
    size_t bufferLength;
    packageToBuffer(package, buffer, bufferLength);
    
    Serial.println("Sending to RF95 Mesh Node \"" + String(nodeIdCenter) + "\"!");

    unsigned long send_start = millis();
    unsigned long timeout = 5000;
    int errorLog;

    while ((millis() - send_start) < timeout)
    {
        errorLog = manager->sendtoWait(buffer, bufferLength, nodeIdCenter);
        if (errorLog == RH_ROUTER_ERROR_NONE)
        { 
            unsigned long send_end = millis();
            unsigned long send_time = send_end - send_start;
            Serial.println("Sending time: " + String(send_time) + " ms");
        
            return 1;
        }
        else
        {
            Serial.println("sendtoWait failed with error code: " + String(errorLog));
            Serial.println("Are the intermediate mesh nodes running in coverage area?");
            return 0;
        }
    }
    Serial.println("LoRa init failed!");

    return -1;
}

void readData(int listenTimeout, DeviceManager &deviceManager,StationManager &stationManager) {
    uint8_t len = sizeof(loraBuf);
    uint8_t nodeIdFrom;
    if (manager->recvfromAckTimeout(loraBuf, &len, listenTimeout, &nodeIdFrom)) {
        packageData package;
        bufferToPackage(loraBuf, len, package);

        Serial.print("Received data from node ");
        Serial.println(nodeIdFrom);

        if (package.title == "warning") {
            Serial.println("Title: warning");
            Serial.println(package.data[0]);
            package.title="batloa";
            sendData(package,3);
            // for (size_t i = 0; i < package.data.size(); ++i) {
            //     Serial.println(package.data[i]);
            //     Serial.print(" ");
            // }
            // Serial.println();
        } else if (package.title == "heartbeat") {
            Serial.println("Title: Heartbeat");
            String nodeIdStr = String(nodeIdFrom);
             for (int i = 1; i <= 3; ++i) {
                    if (i - 1 < package.data.size()) {
                        Serial.print(nodeIdStr +"-"+ String(i)+":");
                        Serial.println(package.data[i - 1]);
                        //deviceManager.updateDevice(nodeIdStr+"-"+String(i), package.data[i - 1]);
                    }
             }
            if (searchID(nodeIdStr, deviceManager)) {
                for (int i = 1; i <= 3; ++i) {
                    if (i - 1 < package.data.size()) {
                        Serial.print(nodeIdStr + String(i)+"-");
                        Serial.println(package.data[i - 1]);
                        deviceManager.updateDevice(nodeIdStr +"-"+ String(i), package.data[i - 1]);
                    } 
                    // else {
                    //     Serial.println("Error: Insufficient data for heartbeat update.");
                    //     return;
                    // }
                    stationManager.updateTime(nodeIdStr);
                    // saveDevicesToSPIFFS(deviceManager);
                }
                // mqtt send
                String jsonBuffer = "{"
                                        "\"access_token\": \"" + tokenAccess + "\","
                                        "\"sensorDevice\": {"
                                        "\"waterlevel\": \"" + String(package.data[0]) + "\","
                                        "\"ntu\": \"" + String(package.data[1]) + "\","
                                        "\"persen\": \"" + String(package.data[2]) + "\","
                                        "}"
                                        "}";
                    mqttSend(jsonBuffer);

            }
             else {
                stationManager.addStation(Station(nodeIdStr,package.data.size()+1,"scan"));
               
            }
        }
       else if(package.title.substring(0, 7) == "register") {
            package.title = package.title.substring(8);
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, package.title.c_str());

            if (error) {
                Serial.println("convert json failed!");
            } else {
                JsonArray devicesArray = doc["devices"].as<JsonArray>();
                
                for (JsonObject deviceObj : devicesArray) {
                    String idD = deviceObj["id"].as<String>();
                    String name = deviceObj["name"].as<String>();
                    String unit = deviceObj["unit"].as<String>();
                    // Thêm vào DeviceManager
                    deviceManager.addDevice(Device(idD, (String)nodeIdFrom, name, 0.0, unit, "unknown"));
                }
            }
        }
        else {
            Serial.println("Unknown title");
            Serial.println();
        }
    }
}

bool searchID(const String& nodeIDForm, const DeviceManager& deviceManager) {
    for (const auto& device : deviceManager.getDevices()) {
        if (nodeIDForm == device.idNode)
            return true;
    }
    return false;
}
