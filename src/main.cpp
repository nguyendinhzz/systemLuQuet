#include <SPI.h>
#include "../lib/system/system.h"
#include "../lib/communication/communication.h"
#if  SPEAKER_STATION
    #include "heartbeat.h"
#endif

unsigned long timeloop=0;
bool checkUART=true;
unsigned long timeP=0;
bool checkBuzzer=false;
unsigned long timeWdtRsPre=0;
void setup() {
    setupDevices();
    runTask1();  
}
void loop() {
    if(millis()-timeWdtRsPre>=1000){
        timeWdtRsPre=millis();
        esp_task_wdt_reset();
    }
    #if SPEAKER_STATION
        processLoop();
    #else
        receiveUART();
    #endif
    if (Serial.available() > 0) {
        //check
        String inputMessage = Serial.readStringUntil('\n');
        Serial.println("Title: Thong so");  
        PackageUART package;
        package.to=2;
        package.data[0]=0x54;
        package.data[1]=210;
        package.data[2]=50;
        package.dataSize=3;
        sendData(package);

    }
    // if(millis()-timeloop>5000){
    //     timeloop=millis();
        
    //     PackageUART package;
    //     package.to=6;
    //     package.data[0]=0x54;
    //     package.data[1]=210;
    //     package.data[2]=50;
    //     package.dataSize=3;
    //     sendData(package);
    // }
    // if (Serial.available() > 0) {
    //     //check
    //     String inputMessage = Serial.readStringUntil('\n');
    //     Serial.println("Title: Thong so");  

    // }
}
