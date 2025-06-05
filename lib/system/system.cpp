
#include "system.h"

float RF95_FREQ;
int nodeSignal;

int nodeIdSelf=3;// khai bao id

RH_RF95 driver(RFM95_CS, RFM95_INT);
RHMesh *manager;
QMC5883LCompass compass;
void setupDevices() {
    
    Log.begin(115200);
    setupLora();
    Wire.begin(I2C_SDA, I2C_SCL);
    #if SPEAKER_STATION
    Log.println("Enable tram loa cảnh báo");
    pinMode(PIN_WARN, OUTPUT);
    #else 
    Port.begin(9600); //RX 17 TX 16
    #endif
    esp_task_wdt_init(WDT_TIMEOUT, true);  // true = Reset ESP khi timeout
    esp_task_wdt_add(NULL);  // Đăng ký Task loop() với WDT
    compass.init();
    Log.print("Set up end");
    
}

void setupLora() {
    Log.print("Node id self: ");
    Log.println(nodeIdSelf);
    RF95_FREQ=410;
    nodeSignal=23;
    pinMode(RFM95_RST, OUTPUT);

    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    if (!driver.init()) {
        Log.println("LoRa radio init failed");
    } else Log.println("LoRa radio init OK!");

    if (!driver.setFrequency(RF95_FREQ)) {
        Log.println("SetFrequency failed");
    } else {
        Log.println("SetFrequency: " + String(RF95_FREQ));
    }

    manager = new RHMesh(driver, nodeIdSelf);

    if (!manager->init()) {
        Log.println(F("Init failed"));
    } else Log.println("Mesh Node \"" + String(nodeIdSelf) + "\" Up and Running!");

    driver.setTxPower(nodeSignal, false);
    driver.setCADTimeout(2000);
     // Timeout 2 giây (Giảm xuống 1000 nếu muốn phản hồi nhanh)

    
}
