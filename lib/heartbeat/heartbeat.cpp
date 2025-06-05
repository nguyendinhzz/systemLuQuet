#include "heartbeat.h"
#include "../measure/measure.h"
#include "../communication/communication.h"

unsigned long timeP=0;

void sendStationState(float a,float b){
    if(millis()-timeP>5001){
        timeP=millis();
        Serial.println("Heart beart");
            Serial.println(a);
            Serial.println(b);
        
    }
    
    
}
