#include "system.h"

SoftwareSerial mySerial(RX, TX); // RX, TX
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setupDevices() {
    
    mySerial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BATERY_PIN,INPUT);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Start");
    saveFloatToEEPROM(4,220);
    Serial.print("Nguong: ");

}
void saveFloatToEEPROM(int address, float value) {
    byte *p = (byte *)(void *)&value;
    for (int i = 0; i < sizeof(value); i++) {
        EEPROM.write(address + i, *p++);
    }
}
float readFloatFromEEPROM(int address) {
    float value = 0.0;
    byte *p = (byte *)(void *)&value;
    for (int i = 0; i < sizeof(value); i++) {
        *p++ = EEPROM.read(address + i);
    }
    return value;
}
