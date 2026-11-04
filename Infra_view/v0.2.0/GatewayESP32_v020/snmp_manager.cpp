#include "snmp_manager.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SNMP_Agent.h>

#include "espnow.h"

WiFiUDP udp;

SNMPAgent snmp("public", "private");

int temp1OID = 0;
int temp2OID = 0;

int statusOID = 0; //status do sensor | 0=OFFLINE, 1=ONLINE

int rssiOID = 0;

uint32_t packetCountOID = 0;

uint32_t sensorUptimeOID = 0;

const std::string getSensorMac()
{
    return std::string(
        sensorMAC.c_str()
    );
}

ValueCallback *temp1Handler = nullptr;
ValueCallback *temp2Handler = nullptr;
ValueCallback *statusHandler = nullptr;
ValueCallback *rssiHandler = nullptr;
ValueCallback *gatewayMacHandler = nullptr;
ValueCallback *packetCountHandler = nullptr;
ValueCallback *sensorMacHandler = nullptr;
ValueCallback *sensorUptimeHandler = nullptr;

std::string gatewayMacOID;
//std::string sensorMacOID;

void initSNMP()
{
    udp.begin(161);

    snmp.setUDP(&udp);

    gatewayMacOID =
        WiFi.macAddress().c_str();

    temp1Handler = snmp.addIntegerHandler(
        ".1.3.6.1.4.1.99999.1.1.0",
        &temp1OID
    );

    temp2Handler = snmp.addIntegerHandler(
        ".1.3.6.1.4.1.99999.1.2.0",
        &temp2OID
    );

    statusHandler = snmp.addIntegerHandler(
        ".1.3.6.1.4.1.99999.1.3.0",
        &statusOID
    );

    rssiHandler = snmp.addIntegerHandler(
        ".1.3.6.1.4.1.99999.1.4.0",
        &rssiOID
    );

    packetCountHandler = snmp.addCounter32Handler(
        ".1.3.6.1.4.1.99999.1.5.0",
        &packetCountOID
    );

    sensorUptimeHandler = snmp.addTimestampHandler(
        ".1.3.6.1.4.1.99999.1.6.0",
        &sensorUptimeOID
    );

    gatewayMacHandler = snmp.addReadOnlyStaticStringHandler(
        ".1.3.6.1.4.1.99999.1.7.0",
        gatewayMacOID
    );

    /*sensorMacHandler = snmp.addReadOnlyStaticStringHandler(
        ".1.3.6.1.4.1.99999.1.8.0",
        sensorMacOID
    );*/
    sensorMacHandler =
    snmp.addDynamicReadOnlyStringHandler(
        ".1.3.6.1.4.1.99999.1.8.0",
        getSensorMac
    );

    snmp.sortHandlers();

    snmp.begin();

    Serial.println();
    Serial.println("=== SNMP ===");

    Serial.println("Community RO: public");
    Serial.println("Community RW: private");

    Serial.println("OID Temp1 : .1.3.6.1.4.1.99999.1.1.0");
    Serial.println("OID Temp2 : .1.3.6.1.4.1.99999.1.2.0");
    Serial.println("OID Status: .1.3.6.1.4.1.99999.1.3.0");
    Serial.println("OID RSSI   : .1.3.6.1.4.1.99999.1.4.0");
    Serial.println("OID PKTS   : .1.3.6.1.4.1.99999.1.5.0");
    Serial.println("OID UPTIME : .1.3.6.1.4.1.99999.1.6.0");
    Serial.println("OID GW MAC : .1.3.6.1.4.1.99999.1.7.0");
}

void handleSNMP()
{
    temp1OID =
        (int)(packet.temp1 * 100.0);

    temp2OID =
        (int)(packet.temp2 * 100.0);

    statusOID =
        sensorOffline ? 0 : 1;

    rssiOID =
        lastRSSI;

    packetCountOID =
        packetsReceived;

sensorUptimeOID =
    packet.uptime;    

    snmp.loop();
}