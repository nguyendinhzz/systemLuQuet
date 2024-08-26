
#include "../lib/system/system.h"
#include "../lib/communication/communication.h"
#include "../lib/devicemanager/devicemanager.h"
#include "../lib/stationmanager/stationmanager.h"
packageData package;
DeviceManager deviceManager;
StationManager stationManager;

unsigned long TIME_TOUCH_CONFIG=0;
unsigned long TIME_CHECK_STATUS=0;
void setup() {
    Serial.begin(115200);
    package.data.resize(2);
    Serial.println("Setup end");
    loadDevicesFromSPIFFS(deviceManager);
    stationManager.addStation(Station("193",3,"online"));
    stationManager.addStation(Station("126",3,"online"));
    stationManager.addStation(Station("82",3,"scan"));
    deviceManager.addDevice(Device("01", "tram 1", "cam bien muc nuoc", 135, "cm", "dung"));
        deviceManager.addDevice(Device("02", "tram 1", "toc do thay doi", 2, "cm/s", "sai"));
        deviceManager.addDevice(Device("03", "tram 1", "pin", 35, "%", "sai"));
    setupDevices();
    setupMQTT();
    
}

void loop() {
    lv_timer_handler();
    reconnectMQTT();
    if(millis()-TIME_TOUCH_CONFIG>2000){
        readData(500,deviceManager,stationManager);
    }
    if(millis()-TIME_CHECK_STATUS>5000){
        TIME_CHECK_STATUS=millis();
        stationManager.checkTimeoutStaion();
    }
    if (Serial.available() > 0) {
        //check
        // saveDevicesToSPIFFS(deviceManager);
        String inputMessage = Serial.readStringUntil('\n');
        package.title="batloa";
        sendData(package,3);
        
        
    }
}
