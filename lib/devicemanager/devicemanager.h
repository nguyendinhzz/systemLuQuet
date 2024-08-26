#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <Arduino.h>
#include <vector>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "../devicemanager/devicemanager.h" 
class Device {
public:
    String id;
    String idNode;
    String name;
    float parameters;
    String unit;
    String status;
    
    Device(String id, String idNode, const String& name, const float &parameters,const String &unit, const String& status);
};

class DeviceManager {
private:
    std::vector<Device> devices;

public:
    std::vector<Device> getDevices() const;
    void addDevice(const Device& device);
    void updateDevice(String id, const float &parameters);
    void clearAllDevices();
    void removeDevicesByIdNode(const String& idNode);
};


void saveDevicesToSPIFFS(DeviceManager &deviceManager);
void loadDevicesFromSPIFFS(DeviceManager &deviceManager);
#endif // DEVICEMANAGER_H
