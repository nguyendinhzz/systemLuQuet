#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

#include <WiFi.h>
#include <esp_wifi.h>
#include <EEPROM.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

#include "ui.h"
extern int nodeIdSelf;
extern RH_RF95 driver;
extern RHMesh *manager;

extern char ssid[32];
extern char passW[32];
extern bool connectWifi;

void setupDevices();
void setupLora();
bool setupWifi();
void saveWiFiCredentials();
void loadWiFiCredentials();

extern TFT_eSPI tft;
extern lv_disp_draw_buf_t draw_buf;
extern lv_color_t buf[];

void setupScreen();

#endif
