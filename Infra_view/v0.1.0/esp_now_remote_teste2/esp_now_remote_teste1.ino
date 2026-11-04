#include <WiFi.h>
#include <esp_now.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

typedef struct
{
  uint32_t id;

  float temp1;
  float temp2;

  uint32_t uptime;

} SensorPacket;

SensorPacket packet;

uint8_t gatewayMAC[] =
{
  0xF8, 0xB3, 0xB7,
  0x21, 0x2F, 0x04
};

void onDataSent(const uint8_t *mac_addr,
                esp_now_send_status_t status)
{
  Serial.print("[ESP-NOW] Envio: ");

  if (status == ESP_NOW_SEND_SUCCESS)
    Serial.println("SUCESSO");
  else
    Serial.println("FALHA");
}

void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("=== SENSOR ===");

  sensors.begin();

  WiFi.mode(WIFI_STA);

  delay(500);

  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo = {};

  memcpy(peerInfo.peer_addr, gatewayMAC, 6);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Erro ao adicionar peer");
    return;
  }

  Serial.println("Peer adicionado");
}

void loop()
{
  sensors.requestTemperatures();

  packet.id = 1;

  packet.temp1 =
      sensors.getTempCByIndex(0);

  packet.temp2 =
      sensors.getTempCByIndex(1);

  packet.uptime =
      millis() / 1000;

  Serial.println("--------------");

  Serial.printf(
      "Temp1: %.2f°C\n",
      packet.temp1);

  Serial.printf(
      "Temp2: %.2f°C\n",
      packet.temp2);

  esp_now_send(
      gatewayMAC,
      (uint8_t *)&packet,
      sizeof(packet));

  delay(10000);
}