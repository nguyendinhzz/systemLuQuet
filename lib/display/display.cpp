#include "display.h"
#include <LiquidCrystal_I2C.h>
#include "../system/system.h"

void printLCD(String str, int a, int b)
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(a, b);
    lcd.print(str);
    
}