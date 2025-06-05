#include "communication.h"
#define END_BYTE   0x03
unsigned long TIME_SEND = 0;
unsigned long TIME_OUT_BTN=0;
unsigned long btnpressTime1 = 0;
unsigned long btnpressTime2 = 0;
unsigned long timeOutConfig=60000;
bool btnAPBefore=0;
unsigned long timePress=0;
void processLoop() {
    if (millis() - TIME_SEND > 15001) {
        TIME_SEND=millis();
        Serial.println("heart beat");
        uint8_t buf[3];
        buf[0]=0x54;
        buf[1]=(uint8_t)round(levelMedium);
        buf[2]=(uint8_t)rateBatery;
        sendPacket(NODE_ID_SELF,NODE_ID_CENTER,buf,3);
    }
   
}

// void sendUART(String tile){
//         mySerial.print(tile);
//         mySerial.print(",");
//         mySerial.print(levelMedium);
//         mySerial.print(",");
//         mySerial.print(rateTurbi);
//         mySerial.print(",");
//         mySerial.println(rateBatery);
// }

void receiveUART() {
    if (mySerial.available() > 0) {
        if (mySerial.read() != START_BYTE) {
            return;
        }
        // Đọc header (form, to, dataSize)
        uint8_t header[3];
        if (mySerial.readBytes(header, 3) != 3) {
            Serial.println("Loi doc header!");
            return;
        }

        PackageUART package;
        package.form = header[0];    // fromID
        package.to = header[1];      // toID
        package.dataSize = header[2]; // dataSize

        // Kiểm tra kích thước dữ liệu
        if (package.dataSize > MAX_DATA_SIZE) {
            Serial.println("Kich thuoc du lieu vuot qua gioi han!");
            return;
        }
        if (mySerial.readBytes(package.data, package.dataSize) != package.dataSize) {
            Serial.println("Loi doc du lieu!");
            return;
        }
        uint8_t receivedChecksum;
        if (mySerial.readBytes(&receivedChecksum, 1) != 1) {
            Serial.println("Loi doc checksum!");
            return;
        }
        uint8_t calculatedChecksum = calculateCheckSum(
            package.form, 
            package.to, 
            package.data,
            package.dataSize
        );

        if (calculatedChecksum != receivedChecksum) {
            Serial.print("Loi: Checksum khong khop! Expected: ");
            Serial.print(calculatedChecksum);
            Serial.print(", Received: ");
            Serial.println(receivedChecksum);
            return;
        }
        Serial.print("Data nhan duoc: ");
        Serial.println(package.data[0]);
        if(package.data[0]==0x42){
            package.data[0]-=1;
            sendPacket(package.to,package.form,package.data,2);
            Serial.print("Thuc hien cau hinh!");
            Serial.println(package.data[1]);
            saveFloatToEEPROM(4,package.data[0]);
            levelDanger=package.data[0];
        }
        
    }
}

void sendPacket(uint8_t form, uint8_t to, const uint8_t* data,uint8_t dataSize) {
    
    if (dataSize > MAX_DATA_SIZE) {
        Serial.println("Kích thước dữ liệu vượt quá giới hạn!");
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

    mySerial.write(packet, index);
    
    // Serial.println("Gói tin gửi thành công!");
}

uint8_t calculateCheckSum(uint8_t form, uint8_t to, const uint8_t* data, uint8_t dataSize) {
    uint8_t sum = form + to + dataSize;
    for (int i = 0; i < dataSize; i++) {
        sum += data[i];
    }
    return sum;
}
// void receiveUART() {
//     if (mySerial.available() > 0) {
//         String receivedData = mySerial.readStringUntil('\n');
//         int commaIndex1 = receivedData.indexOf(','); 
//         int commaIndex2 = receivedData.indexOf(',', commaIndex1 + 1);
//         int commaIndex3 = receivedData.indexOf(',', commaIndex2 + 1);
        
//         String tile = receivedData.substring(0, commaIndex1);
//         String strVar1 = receivedData.substring(commaIndex1 + 1, commaIndex2);
//         String strVar2 = receivedData.substring(commaIndex2 + 1, commaIndex3);
//         String strVar3 = receivedData.substring(commaIndex3 + 1);
//         Serial.println(tile);
       
//         if(tile =="ip"){
//                 Serial.println(strVar1);
//                 lcd.clear();
//                 lcd.print(strVar1);
//                 digitalWrite(LEDPIN1,1);
//                 Serial.println(receivedData);
//                 delay(15000);
//             }
//         else if(tile =="endip"){
//             digitalWrite(LEDPIN1,0);
//         }
//         else if(tile=="thongso"){
//             sendUART(0x54);// xác nhận cảnh báo lần 2
//         }
//         else if (commaIndex1 > 0 && commaIndex2 > commaIndex1 && commaIndex3 > commaIndex2) {

//             if (tile == "change") {
//                 bottomToSensor = strVar1.toFloat();
//                 levelDanger = strVar2.toFloat();
//                 rateTurbi = strVar3.toFloat();
//                 saveFloatToEEPROM(0, bottomToSensor);
//                 saveFloatToEEPROM(4, levelDanger);
//                 saveFloatToEEPROM(8, rateTurbi);
//             }
            
//         }
//     }
// }

