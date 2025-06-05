#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <Wire.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <esp_task_wdt.h>
#include "esp_system.h"

#define RFM95_CS   5   // Chọn CS mới
#define RFM95_RST  21  // Chân Reset
#define RFM95_INT  4   // Chân ngắt (Interrupt)
#define PIN_WARN   2   // LED cảnh báo
#define I2C_SDA 17
#define I2C_SCL 22
// VCC	3.3V	Cấp nguồn 3.3V
// GND	GND	Mass (đất)
// SCK	GPIO18	Clock SPI (SCL)
// MISO	GPIO19	Master In, Slave Out
// MOSI	GPIO23	Master Out, Slave In
// NSS (CS)	GPIO5	Chip Select (CS)
// RST	GPIO21	Reset module
// DIO0 (INT)	GPIO4	Ngắt (Interrupt)

#define ENABLE_LOG 1
#if ENABLE_LOG
  #define Log Serial
#else
  #define Log if (false) Serial  // Khi tắt log, tất cả lệnh Log sẽ không làm gì
#endif
#define Port Serial2 // Select port

#define SPEAKER_STATION 1
#define WDT_TIMEOUT 30  // Timeout watchdog (giây)

extern int nodeIdSelf;

extern String macID;
extern RH_RF95 driver;
extern RHMesh *manager;

#include <QMC5883LCompass.h>

extern QMC5883LCompass compass;

void setupDevices();
void setupLora();

#endif // SYSTEM_H
