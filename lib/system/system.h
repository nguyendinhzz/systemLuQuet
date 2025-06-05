#ifndef SYSTEM_H
#define SYSTEM_H

#define TRIG_PIN 2
#define ECHO_PIN 3

#define RX 6 // conn 16
#define TX 5 // conn 17
//A4 A5
#define BATERY_PIN A1
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
extern SoftwareSerial mySerial;
extern LiquidCrystal_I2C lcd;

void setupDevices();
void saveFloatToEEPROM(int address, float value);
float readFloatFromEEPROM(int address);
#endif // SYSTEM_H
