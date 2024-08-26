#include "system.h"

#define RFM95_CS 25
#define RFM95_RST 26
#define RFM95_INT 27
#define RF95_FREQ 915.0

int nodeIdSelf;
RH_RF95 driver(RFM95_CS, RFM95_INT);
RHMesh *manager;

bool connectWifi =false;

char ssid[32];
char passW[32];
extern long TIME_TOUCH_CONFIG;
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
TFT_eSPI tft = TFT_eSPI(screenHeight, screenWidth);
lv_disp_draw_buf_t draw_buf;
lv_color_t buf[screenWidth * screenHeight / 10];

void setupDevices() {
    EEPROM.begin(64);
    loadWiFiCredentials();
    setupWifi();
    setupLora();
    setupScreen();
}

void setupLora(){
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    if (ret == ESP_OK) {
            nodeIdSelf= (baseMac[4]*baseMac[5])%255;
    Serial.print("Node id self: ");
    Serial.println(nodeIdSelf);
  } else {
    Serial.println("Failed to read MAC address");
  }
    
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    if (!driver.init()) {
        Serial.println("LoRa radio init failed");
    }
    else Serial.println("LoRa radio init OK!");

    if (!driver.setFrequency(RF95_FREQ)) {
        Serial.println("SetFrequency failed");
    }
    else Serial.println("SetFrequency: \""+String(RF95_FREQ));

    manager = new RHMesh(driver, nodeIdSelf);
    
    if (!manager->init()) {
        Serial.println(F("Init failed"));
    } else Serial.println("Mesh Node \"" + String(nodeIdSelf) + "\" Up and Running!");

    driver.setTxPower(23, false);
    driver.setCADTimeout(500);
}

bool setupWifi(){
    WiFi.begin(ssid, passW);
    Serial.print("Connecting to ");
    Serial.print(ssid);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (millis() - startTime > 10000) { // 10 giây timeout
        Serial.println("Dont connect router you should config wifi on tft touch!");
        connectWifi=false;
        return false;
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("Connected router! IP address: ");
        connectWifi=true;
        Serial.println(WiFi.localIP());
        return true;
  }
  return false;
}

//   String newSSID = server.arg("ssid");
//   String newPass = server.arg("pass");

//   if (newSSID.length() > 0 && newPass.length() > 0) {
//     newSSID.toCharArray(ssid, 32);
//     newPass.toCharArray(passW, 32);
//     saveWiFiCredentials();

void saveWiFiCredentials() {
  for (int i = 0; i < 32; ++i) {
    EEPROM.write(i, ssid[i]);
    EEPROM.write(32 + i, passW[i]);
  }
  EEPROM.commit();
}

void loadWiFiCredentials() {
  for (int i = 0; i < 32; ++i) {
    ssid[i] = EEPROM.read(i);
    passW[i] = EEPROM.read(32 + i);
  }
  ssid[31] = '\0';
  passW[31] = '\0';
}

void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX = 0, touchY = 0;

    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        TIME_TOUCH_CONFIG=millis();
        data->state = LV_INDEV_STATE_PR;
        data->point.x = screenWidth - touchX;
        data->point.y = touchY;
        // Serial.print("x: ");
        // Serial.print(screenWidth - touchX);
        // Serial.print(", y: ");
        // Serial.println(touchY);

    }
}

void setupScreen()
{
    lv_init();
    tft.begin();          
    tft.setRotation( 3 ); 
    
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    ui_init();
}
