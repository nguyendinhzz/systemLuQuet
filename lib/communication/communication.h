#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "../system/system.h"
#include "../measure/measure.h"
#define START_BYTE 0x7E            // Byte bắt đầu
#define DEFAULT_SIZE 5             // Kích thước mặc định (form, to, title, dataSize, checksum)
#define MAX_DATA_SIZE 10           // Kích thước tối đa dữ liệu
#define MAX_PACKET_SIZE 15        // Kích thước tối đa gói tin
#define NODE_ID_SELF 2
#define NODE_ID_CENTER 6

struct PackageUART {
    uint8_t form;          // Địa chỉ nguồn
    uint8_t to;            // Địa chỉ đích
    uint8_t data[MAX_DATA_SIZE];  // Dữ liệu
    uint8_t dataSize;      // Số lượng byte dữ liệu
    uint8_t checkSum;      // Kiểm tra lỗi (8-bit)
};

void processLoop();
void receiveUART();
void sendPacket(uint8_t form, uint8_t to, const uint8_t* data, uint8_t dataSize);
uint8_t calculateCheckSum(uint8_t form, uint8_t to, const uint8_t* data, uint8_t dataSize);
#endif // COMMUNICATION_H
