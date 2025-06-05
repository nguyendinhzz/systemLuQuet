#include <SPI.h>
#include "../lib/measure/measure.h"
#include "../lib/heartbeat/heartbeat.h"
#include "../lib/system/system.h"
#include "../lib/communication/communication.h"

unsigned long timeLoop=0;
unsigned long timeLed=0;
bool ledState=false;
void setup() {
    
    Serial.begin(9600);
    setupDevices();
    Serial.println("Setup end");
}

void loop() {
    measureProcess();
    processLoop();
    receiveUART();

    // while(millis()-timeLoop>5000){//confirm timeout
    //     if (mySerial.available() > 0) {
    //     char cmd = mySerial.read();
    //     Serial.println(cmd, HEX); // In ra dưới dạng mã hex để kiểm tra chính xác dữ liệu nhận được
    //     if (cmd == 0x4F) {
    //         Serial.println("ket thuc");
    //         return;
    //     }
    //     }

    
    // }
        
   
    
}
