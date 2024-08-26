#include "DeviceManager.h"

Device::Device(String id, String idNode, const String& name, const float &parameters, const String& unit,const String& status)
    : id(id), idNode(idNode), name(name), parameters(parameters),unit(unit), status(status) {}

void DeviceManager::addDevice(const Device& device) {
    devices.push_back(device);
}

void DeviceManager::updateDevice(String id, const float &parameters) {
    for (auto& device : devices) {
        if (device.id == id) {
            device.parameters = parameters;
            if(parameters==-1)
            device.status = "ERROR";
            else  device.status= "ACTIVE";
            break;
        }
    }
}

std::vector<Device> DeviceManager::getDevices() const {
    return devices;
}

//process
void saveDevicesToSPIFFS(DeviceManager &deviceManager) {
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS");
        return;
    }
    DynamicJsonDocument doc(2048); // Adjust size as necessary
    JsonArray devicesArray = doc.createNestedArray("devices");

    for (const Device& device : deviceManager.getDevices()) {
        JsonObject deviceObj = devicesArray.createNestedObject();
        deviceObj["id"] = device.id;
        deviceObj["idNode"] = device.idNode;
        deviceObj["name"] = device.name;
        deviceObj["parameters"] = device.parameters;
        deviceObj["unit"] = device.unit;
        deviceObj["status"] = device.status;
    }
    File file = SPIFFS.open("/devices.json", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    serializeJson(doc, file);
    file.close();

    Serial.println("Devices saved to SPIFFS");
}

void loadDevicesFromSPIFFS(DeviceManager &deviceManager) {
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS");
        return;
    }

    File file = SPIFFS.open("/devices.json", FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    DynamicJsonDocument doc(2048); // Adjust size as necessary

    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.println("Failed to read file, using default configuration");
        file.close();
        return;
    }
    file.close(); // Close the file after reading

    deviceManager.getDevices().clear();
    JsonArray devicesArray = doc["devices"].as<JsonArray>();
    for (JsonObject deviceObj : devicesArray) {
        String id = deviceObj["id"];
        String idNode = deviceObj["idNode"];
        String name = deviceObj["name"];
        float parameters = deviceObj["parameters"];
        String unit = deviceObj["unit"];
        String status = deviceObj["status"];

        deviceManager.addDevice(Device(id, idNode, name, parameters, unit, status));
    }
    Serial.println("Devices loaded from SPIFFS");
}
void DeviceManager::removeDevicesByIdNode(const String& idNode) {
    devices.erase(
        std::remove_if(devices.begin(), devices.end(), [&](const Device& device) {
            return device.idNode == idNode;
        }),
        devices.end()
    );
}
void DeviceManager::clearAllDevices() {
    devices.clear();
}