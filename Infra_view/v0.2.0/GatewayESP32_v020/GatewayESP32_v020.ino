//#include <WebServer.h>

#include "config.h"
#include "espnow.h"
#include "web_server.h"
#include "snmp_manager.h"

void connectWiFi()
{
    WiFi.mode(WIFI_STA);

    delay(500);

    Serial.print(
        "MAC Gateway: ");

    Serial.println(
        WiFi.macAddress());

    Serial.println();
    Serial.print(
        "Conectando em ");

    Serial.println(
        WIFI_SSID);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();
    Serial.println(
        "WiFi conectado");

    Serial.print(
        "IP: ");

    Serial.println(
        WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println(
        "=== GATEWAY ===");

    connectWiFi();

    initEspNow();

    initWebServer();

    initSNMP();
}

void loop()
{
    handleWebServer();

    handleSNMP();

    
    if (
        lastPacket > 0 &&
        millis() - lastPacket >
        SENSOR_TIMEOUT)
    {
        if (!sensorOffline)
        {
            sensorOffline = true;

            Serial.println();
            Serial.println(
                "####################");

            Serial.println(
                "[ALARME] SENSOR OFFLINE");

            Serial.println(
                "####################");
        }
    }

    delay(100);
}