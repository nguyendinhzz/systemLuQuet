#include "measure.h"
#include "../system/system.h"

const unsigned long TIME_MEASURE_SENSOR = 400;  // thời gian đo lường mực nước từ cảm biến
unsigned long PREVIOUS_TIME_MEASURE_SENSOR = 0;
const unsigned long TIME_MEDIUM_LEVEL = 2000;   // thời gian tính toán mực nước trung bình hiện tại
unsigned long PREVIOUS_TIME_MEDIUM_LEVEL = 0;
unsigned long PREVIOUS_MINUTE_TIME = 0;
unsigned long MINUTE_TIME = 15000;
//
unsigned long lastWarningTime = 0;
const unsigned long WARNING_INTERVAL = 4500;
// bien extern
float bottomToSensor = 250.0;
float levelDanger = readFloatFromEEPROM(4);

float rateChangeDanger = 4;
float levelMedium = 0.0;  
float rateChange = 0.0;
// set cung
int rateTurbi=0;
int rateBatery = 0;
int voltSolar=0;

bool sensorError=false;
bool warningFlood = false;
int bounceCount = 0;
const float bounceValue = 5.0;
const int minBounceCount = 2;  // số lần tối thiểu để chấp nhận giá trị mực nước

const int sizeArray = 5;
float levelArray[sizeArray];
float levelMinuteBefore = -1;
float sumLevelMinute = 0;
float sumLevel = 0;
float validLevel = 0;
int k = 0;
float waterLevelBefore = -1;
float currentLevel;
int mediumCount = 0;

//
const int QUEUE_SIZE = 15;
float levelQueue[QUEUE_SIZE];
int queueIndex = 0;
int queueCount = 0;
float sumQueue = 0.0;
float movingAverage = -1;
void measureProcess() {
    if (millis() - PREVIOUS_TIME_MEASURE_SENSOR >= TIME_MEASURE_SENSOR) {
        PREVIOUS_TIME_MEASURE_SENSOR = millis();
        
        currentLevel = waterLevel();

        levelArray[k] = currentLevel;
        k = (k + 1) % sizeArray;
        
        if (movingAverage > 0 && abs(movingAverage - currentLevel) >= bounceValue) {
            bounceCount++;
        } else {
            validLevel += currentLevel;
            if (bounceCount < minBounceCount)
                bounceCount = 0; // reset bounce count
        }
    }

    if (millis() - PREVIOUS_TIME_MEDIUM_LEVEL >= TIME_MEDIUM_LEVEL) {
        PREVIOUS_TIME_MEDIUM_LEVEL = millis();
        
        if (movingAverage == -1 || bounceCount >= minBounceCount) {
            for (int i = 0; i < sizeArray; i++) {
                sumLevel += levelArray[i];
            }
            levelMedium = sumLevel / sizeArray;
        } else {
            int valuesC = 0;
            for (int i = 0; i < sizeArray; i++) {
                if (abs(movingAverage - levelArray[i]) < bounceValue) {
                    sumLevel += levelArray[i];
                    valuesC++;
                }
            }
            levelMedium = validLevel / valuesC;
        }
        Serial.println(levelMedium);
        if (movingAverage != -1) {
            rateChange = levelMedium-movingAverage;
        }
        else rateChange=0;
        if (levelMedium < 0||levelMedium==bottomToSensor) {
            levelMedium = 0;
            rateChange = 0;
            if(sensorError){
                sensorError=false;
            }
        }else {
            sensorError=true;
        }
        
        if (levelMedium != 0 &&levelMedium!=bottomToSensor&& (rateChange > rateChangeDanger || levelMedium > levelDanger)) {
            if (millis() - lastWarningTime >= WARNING_INTERVAL) {
                Serial.println("_________Canh bao lu________");
                uint8_t buf[2];
                buf[0]=0x57;
                buf[1]=levelMedium;
                sendPacket(NODE_ID_SELF,NODE_ID_CENTER,buf,sizeof(buf));
                lastWarningTime = millis();
            }
            // sendUART("warning");
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(levelMedium);
        lcd.print(" cm");
        rateBatery=map(analogRead(BATERY_PIN),380,600,0,100);
        if(rateBatery>100||rateBatery<0)
            rateBatery=100;
        Serial.print("Pin: ");
        Serial.println(rateBatery);
        lcd.setCursor(0,1);
        lcd.print(rateBatery);
        lcd.print(" %");
        // Serial.print("update lcd");
        // Serial.println(levelMedium);
        //     Serial.println(rateChange); 
        // Serial.print("So lan dot bien vuot nguong: ");
        // Serial.println(bounceCount);

        // Reset values
        validLevel = 0;
        sumLevel = 0;
        bounceCount = 0;
        if (levelMedium != 0) {
            sumLevelMinute += levelMedium;
            mediumCount++;
        }
        if (levelMedium != 0) {
            if (queueCount == QUEUE_SIZE) {
                sumQueue -= levelQueue[queueIndex];
            } else {
                queueCount++;
            }
            levelQueue[queueIndex] = levelMedium;
            sumQueue += levelMedium;

            queueIndex = (queueIndex + 1) % QUEUE_SIZE;

            // math
            if (queueCount == QUEUE_SIZE) 
            movingAverage = sumQueue / queueCount;
        }
    }


}

float waterLevel() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration * 0.034 / 2;

    float level = bottomToSensor - distance;
    return level;
}
