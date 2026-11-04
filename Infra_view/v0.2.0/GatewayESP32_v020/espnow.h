#ifndef ESPNOW_HANDLER_H
#define ESPNOW_HANDLER_H

#include <Arduino.h>

typedef struct
{
    uint32_t id;

    float temp1;
    float temp2;

    uint32_t uptime;

} SensorPacket;

extern String sensorMAC;

extern SensorPacket packet;

extern int lastRSSI;

extern bool sensorOffline;

extern unsigned long lastPacket;

extern uint32_t packetsReceived;

void initEspNow();

#endif