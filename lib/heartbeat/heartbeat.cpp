#include "heartbeat.h"
#include "communication.h"
#include "system.h"
unsigned long timeResponPre=0;
bool speakerStatus=false;
bool speakerSw=false;
unsigned long timeOffCyle=5000;
unsigned long timeOffCylePre=0;
void processLoop(){
    if(millis()-timeResponPre>10001){
        timeResponPre=millis();
        uint8_t buf[3];
        buf[0]=0x6C;
        buf[1]=speakerStatus;
    }
    if(speakerStatus&&millis()-timeOffCylePre>timeOffCyle){
        timeOffCylePre=0;
        digitalWrite(PIN_WARN,speakerSw);
        speakerSw=!speakerSw;

    }
}