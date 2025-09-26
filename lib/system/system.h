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

#define ENABLE_LOG 0
#if ENABLE_LOG
  #define Log Serial
#else
  #define Log if (false) Serial  // Khi tắt log, tất cả lệnh Log sẽ không làm gì
#endif
#define Port Serial2 // Select port

#define SPEAKER_STATION 0
#define WDT_TIMEOUT 30  // Timeout watchdog (giây)

extern int nodeIdSelf;

extern String macID;
extern RH_RF95 driver;
extern RHMesh *manager;

void setupDevices();
void setupLora();

#endif // SYSTEM_H
