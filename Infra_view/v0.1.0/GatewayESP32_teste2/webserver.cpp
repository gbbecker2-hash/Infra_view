//#include "web_server.h"
#include <WiFi.h>
#include <WebServer.h>

#include <dashboard_html.h>

WebServer server(80);

void handleRoot()
{
    server.send(
        200,
        "text/html",
        getDashboardHTML());
}

void handleApiStatus()
{
    String json = "{";

    json += "\"online\":";
    json += sensorOffline ? "false" : "true";

    json += ",\"sensor_id\":";
    json += String(packet.id);

    json += ",\"temp1\":";
    json += String(packet.temp1, 2);

    json += ",\"temp2\":";
    json += String(packet.temp2, 2);

    json += ",\"rssi\":";
    json += String(lastRSSI);

    json += ",\"packets\":";
    json += String(packetsReceived);

    json += ",\"sensor_uptime\":";
    json += String(packet.uptime);

    json += ",\"last_packet_seconds\":";
    json += String((millis() - lastPacket) / 1000);

    json += ",\"gateway_ip\":\"";
    json += WiFi.localIP().toString();
    json += "\"";

    json += ",\"gateway_mac\":\"";
    json += WiFi.macAddress();
    json += "\"";

    json += ",\"sensor_mac\":\"";
    json += sensorMAC;
    json += "\"";

    json += "}";

    server.send(
        200,
        "application/json",
        json
    );
}

void initWebServer()
{
    server.on(
    "/",
    handleRoot);

    server.on(
    "/api/status",
    handleApiStatus);

    server.begin();

    Serial.println(
        "WebServer iniciado");
}

void handleWebServer()
{
    server.handleClient();
}