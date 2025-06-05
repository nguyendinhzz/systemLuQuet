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
    Log.print("Task1 running on core ");
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
    
   
}
