#ifndef DASHBOARD_HTML_H
#define DASHBOARD_HTML_H

#include "espnow.h"

String getDashboardHTML()
{
    String statusColor =
        sensorOffline ? "red" : "green";

    String statusText =
        sensorOffline ? "OFFLINE" : "ONLINE";

    String html;

    html += R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<meta charset="UTF-8">

<meta http-equiv="refresh"
      content="5">

<title>ESP32 Gateway</title>

<style>

body{
    max-width:300px;
    width:95%;
    font-family:Arial,Helvetica,sans-serif;
    background:#ececec;
    font-size:14px;
    margin:10px auto;
}

.card{
    background:#fff;
    margin-bottom:6px;
    padding:8px 8px;
    border-radius:6px;
    box-shadow:0 1px 3px rgba(0,0,0,.15);
}

h1{
    margin:0;
    font-size:18px;
}

h2{
    margin:0;
    font-size:16px;
}

h3{
    margin:0;
    font-size:14px;
}

table{
    width:100%;
    border-collapse:collapse;
}

td{
    padding:4px;
    border-bottom:1px solid #ddd;
}

.value{
    text-align:right;
    font-weight:bold;
}

</style>

</head>

<body>

)rawliteral";

    html += "<div class='card'>";
    html += "<h1>ESP32 Gateway</h1>";
    html += "</div>";

    html += "<div class='card'>";

    html += "<table>";

    html += "<tr><td>Status</td><td class='value'>";
    html += "<span style='color:" + statusColor + ";'>";
    html += statusText;
    html += "</span>";
    html += "</td></tr>";

    html += "<tr><td>Temp1</td><td class='value'>";
    html += String(packet.temp1,2);
    html += " °C</td></tr>";

    html += "<tr><td>Temp2</td><td class='value'>";
    html += String(packet.temp2,2);
    html += " °C</td></tr>";

    html += "<tr><td>RSSI</td><td class='value'>";
    html += String(lastRSSI);
    html += " dBm</td></tr>";

    html += "<tr><td>MAC Sensor</td><td class='value'>";
    html += sensorMAC;
    html += "</td></tr>";

    html += "<tr><td>Pacotes</td><td class='value'>";
    html += String(packetsReceived);
    html += "</td></tr>";

    html += "<tr><td>IP Gateway</td><td class='value'>";
    html += WiFi.localIP().toString();
    html += "</td></tr>";

    html += "<tr><td>Uptime</td><td class='value'>";
    html += String(packet.uptime);
    html += " s</td></tr>";

    html += "<tr><td>Último pacote</td><td class='value'>";
    html += String((millis() - lastPacket)/1000);
    html += " s</td></tr>";

    html += "</table>";

    html += "</div>";

    html += "</body></html>";

    return html;
}

#endif