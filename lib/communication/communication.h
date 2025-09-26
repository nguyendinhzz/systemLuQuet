#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <RHMesh.h>
#include <vector>
#include "../system/system.h"
           // Byte bắt đầu        
#define START_BYTE 0x7E  // Ký tự bắt đầu
#define DEFAULT_SIZE 3       // Kích thước mặc định (form, to, title, dataSize, checksum)
// Kích thước tối đa dữ liệu
#define MAX_PACKET_SIZE 15        // Kích thước tối đa gói tin
#define MAX_DATA_SIZE 32

struct PackageUART {
    uint8_t form;
    uint8_t to;
    uint8_t dataSize;
    uint8_t data[MAX_DATA_SIZE];
    uint8_t checkSum;
};
void runTask1();
void readContinuity( void * pvParameters );
int sendData(PackageUART package);
void receiveUART();
void sendPacket(uint8_t form, uint8_t to, const uint8_t* data,uint8_t dataSize);
uint8_t calculateCheckSum(uint8_t form, uint8_t to, const uint8_t* data, uint8_t dataSize);
#endif // COMMUNICATION_H
