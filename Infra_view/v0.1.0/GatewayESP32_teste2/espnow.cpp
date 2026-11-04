#include "espnow.h"

#include <WiFi.h>
#include <esp_now.h>

SensorPacket packet;

int lastRSSI = 0;

bool sensorOffline = true;

unsigned long lastPacket = 0;

uint32_t packetsReceived = 0;

String sensorMAC = "";

void onDataRecv(
    const esp_now_recv_info_t *info,
    const uint8_t *incomingData,
    int len)
{
    char macStr[18];

    snprintf(
        macStr,
        sizeof(macStr),
        "%02X:%02X:%02X:%02X:%02X:%02X",
        info->src_addr[0],
        info->src_addr[1],
        info->src_addr[2],
        info->src_addr[3],
        info->src_addr[4],
        info->src_addr[5]
    );
    sensorMAC = macStr;

    Serial.printf("MAC Sensor: %s\n", sensorMAC.c_str());

    memcpy(
        &packet,
        incomingData,
        sizeof(packet));

    packetsReceived++;

    sensorOffline = false;

    lastPacket = millis();

    lastRSSI = info->rx_ctrl->rssi;

    Serial.println();
    Serial.println(
        "===== PACOTE RECEBIDO =====");

    Serial.printf(
        "Sensor ID: %lu\n",
        packet.id);

    Serial.printf(
        "Temp1: %.2f °C\n",
        packet.temp1);

    Serial.printf(
        "Temp2: %.2f °C\n",
        packet.temp2);

    Serial.printf(
        "RSSI: %d dBm\n",
        lastRSSI);

    Serial.printf(
        "Pacotes: %lu\n",
        packetsReceived);
}

void initEspNow()
{
    if (esp_now_init() != ESP_OK)
    {
        Serial.println(
            "Erro ao iniciar ESP-NOW");

        return;
    }

    esp_now_register_recv_cb(
        onDataRecv);

    Serial.println(
        "ESP-NOW iniciado");
}