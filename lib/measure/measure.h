#ifndef MEASURE_H
#define MEASURE_H

#define BTN_PIN 0 // Chọn chân kết nối với nút nhấn
#define AP_TIMEOUT 60000
#include "../communication/communication.h"

extern float levelMedium;
extern float rateChange;
extern float levelDanger;
extern float bottomToSensor;
extern int rateTurbi;
extern int voltSolar;
extern int rateBatery;
extern float rateChangeDanger;
extern bool sensorError;
float readFloatFromEEPROM(int address);
void saveFloatToEEPROM(int address, float value);
void measureProcess();
float waterLevel();

#endif
    