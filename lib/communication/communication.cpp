#include "communication.h"
#define LISTEN_TIMEOUT 1000
extern int nodeIdCenter;
extern RHMesh *manager;
#define SIZE_PACKET_UART sizeof(DataPacket)
bool sendBufStr=false;
unsigned long lastWarningTime = 0; 
unsigned long heartBeartTime=0;
const unsigned long warningInterval = 60000;
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
unsigned long lastTimeSent = 0;
bool checkSendConfirm = false;
TaskHandle_t Task1;
void runTask1(){
    xTaskCreatePinnedToCore(
        readContinuity, 
        "Task1", 
        8192,  // Giảm stack size
        NULL, 
        1,
        &Task1, 
        1
    );    
}

void readContinuity(void *pvParameters) {
    // Log.println(xPortGetCoreID());
    for (;;) {
            uint8_t len = sizeof(buf);
            uint8_t nodeIdFrom;

            if (manager->recvfromAckTimeout(buf, &len, LISTEN_TIMEOUT, &nodeIdFrom)) {
                
                Log.print("Nhan duoc goi tin tu: ");
                Log.println(nodeIdFrom);
                Log.println(buf[0]);
                sendPacket(nodeIdFrom, nodeIdSelf, buf, len);
                
        }
        
        vTaskDelay(20 / portTICK_PERIOD_MS);  // Nhường CPU
    }
}
int sendData(PackageUART packge) {
    unsigned long send_start = millis();
    vTaskDelete(Task1);
    int errorLog = -1;
        Log.println("Gui tap tin");
        errorLog = manager->sendtoWait(packge.data, packge.dataSize, packge.to);
        runTask1();
        if (errorLog == RH_ROUTER_ERROR_NONE) { 
            unsigned long send_time = millis() - send_start;
            Log.println("Sending time: " + String(send_time) + " ms");
            return 1;
        } else {
            Log.println("sendtoWait failed with error code: " + String(errorLog));
            return 0;
        }
}


void receiveUART() {
    if (Port.available() > 0) {
        if (Port.read() != START_BYTE) {
            return;
        }
        // Đọc header (form, to, dataSize)
        uint8_t header[3];
        if (Port.readBytes(header, 3) != 3) {
            Port.println("Loi doc header!");
            return;
        }

        PackageUART package;
        package.form = header[0];    // fromID
        package.to = header[1];      // toID
        package.dataSize = header[2]; // dataSize

        // Kiểm tra kích thước dữ liệu
        if (package.dataSize > MAX_DATA_SIZE) {
            Port.println("Kich thuoc du lieu vuot qua gioi han!");
            return;
        }
        if (Port.readBytes(package.data, package.dataSize) != package.dataSize) {
            Port.println("Loi doc du lieu!");
            return;
        }
        uint8_t receivedChecksum;
        if (Port.readBytes(&receivedChecksum, 1) != 1) {
            Port.println("Loi doc checksum!");
            return;
        }
        uint8_t calculatedChecksum = calculateCheckSum(
            package.form, 
            package.to, 
            package.data,
            package.dataSize
        );

        if (calculatedChecksum != receivedChecksum) {
            Port.print("Loi: Checksum khong khop! Expected: ");
            Port.print(calculatedChecksum);
            Port.print(", Received: ");
            Port.println(receivedChecksum);
            return;
        }
        
            Log.print("Gui goi tin đến: ");
            Log.println(package.to);
            sendData(package);
        
        
    }
}

void sendPacket(uint8_t form, uint8_t to, const uint8_t* data,uint8_t dataSize) {
    
    if (dataSize > MAX_DATA_SIZE) {
        Port.println("Kích thước dữ liệu vượt quá giới hạn!");
        return;
    }
    uint8_t packet[MAX_PACKET_SIZE];
    int index = 0;

    packet[index++] = START_BYTE;
    packet[index++] = form;
    packet[index++] = to;
    packet[index++] = dataSize;

    memcpy(&packet[index], data, dataSize);
    index += dataSize;

    uint8_t sum = calculateCheckSum(form, to, data, dataSize);
    packet[index++] = sum;

    Port.write(packet, index);
    
    // Port.println("Gói tin gửi thành công!");
}

uint8_t calculateCheckSum(uint8_t form, uint8_t to, const uint8_t* data, uint8_t dataSize) {
    uint8_t sum = form + to + dataSize;
    for (int i = 0; i < dataSize; i++) {
        sum += data[i];
    }
    return sum;
}
