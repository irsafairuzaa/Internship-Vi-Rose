#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <string.h>

//MAC address ESP Receiver
uint8_t receiverMac[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

void setup() {
  Serial.begin(115200);
  Serial.println("ESP Bridge Siap...");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW gagal diinisialisasi");
    return;
  }
  
  Serial.println("ESP-NOW siap kirim data");
}

void loop() {
  if (Serial.available()) {
    String chunk = Serial.readStringUntil('\n');
    Serial.println("Terima chunk: " + chunk);

    esp_err_t result = esp_now_send(receiverMac, (uint8_t*)chunk.c_str(), chunk.length());
    if (result == ESP_OK) {
      Serial.println("Chunk dikirim via ESP-NOW");
    } else {
      Serial.println("Gagal kirim chunk");
    }
  }
}