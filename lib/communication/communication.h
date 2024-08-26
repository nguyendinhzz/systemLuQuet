#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include "../system/system.h"
#include "../devicemanager/devicemanager.h"
#include "../stationmanager/stationmanager.h"
#include <PubSubClient.h>
struct packageData {
    String title;
    std::vector<float> data;
};

extern String tokenAccess;
int sendData(const packageData &data, int nodeIdCenter);
void readData(int listenTimeout, DeviceManager &deviceManager,StationManager &stationManager);
bool searchID(const String& nodeIDForm, const DeviceManager& deviceManager);
void setupMQTT();
void reconnectMQTT();
void mqttSend(String jsonStr);
void callback(char* topic, byte* payload, unsigned int length);
#endif // COMMUNICATION_H
