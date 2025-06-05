#include "heartbeat.h"
#include "communication.h"
#include "system.h"
unsigned long timeResponPre=0;
unsigned long timeOffCylePre=0;
unsigned long lastReportTime = 0;
unsigned long TIME_SEND = 0;
float vibrationSum=0;
#define VIBRATION_DANGER 250
float vibrationValue=0;

int offsetX = 0, offsetY = 0, offsetZ = 0;
float scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;

float prev_x = 0, prev_y = 0, prev_z = 0;

void processLoop(){
    
    if(millis()-timeResponPre>=1000){
        timeResponPre=millis();
        compass.read();  // Đọc dữ liệu từ cảm biến liên tục
        float x = compass.getX();
        float y = compass.getY();
        float z = compass.getZ();

        // Tính toán rung động
        vibrationSum += sqrt(
            pow(x - prev_x, 2) + 
            pow(y - prev_y, 2) + 
            pow(z - prev_z, 2)
        );
        prev_x = x;
        prev_y = y;
        prev_z = z;

    }
    if(millis()-timeOffCylePre>5000){
        timeOffCylePre=millis();
        vibrationValue=vibrationSum;
        Serial.println(vibrationSum);
        if(millis()>9999&&vibrationSum>VIBRATION_DANGER){
            PackageUART package;
            package.form=nodeIdSelf;
            package.to=6;
            package.data[0]=0x57;
            package.data[1]=vibrationSum/10;
            package.dataSize=2;
            Log.println("Gui canh bao");
            sendData(package);
            timeOffCylePre=millis();
            vibrationSum=0;
            //  delay(300);

        }
        vibrationSum=0;

    }
    if (millis() - TIME_SEND >= 15000) {
        TIME_SEND=millis();
        PackageUART package;
        package.form=nodeIdSelf;
        package.to=6;
        package.data[0]=0x36;
        package.data[1]=vibrationValue/10;
        package.dataSize=2;
        sendData(package);
        timeOffCylePre=millis();
        TIME_SEND=millis();
        vibrationSum=0;
        timeResponPre=millis();
        // delay(300);
    }
}