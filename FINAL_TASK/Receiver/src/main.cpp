#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <esp_err.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define MAC_ADDRESS_LENGTH 6
#define MAC_ADDRESS_TOTAL 12

uint8_t mac_addresses[MAC_ADDRESS_TOTAL][MAC_ADDRESS_LENGTH] = {
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x10},  // Fauzan Firdaus
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x11},  // Africha Sekar wangi
    {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x10},  // Rafaina Erin Sadia
    {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x11},  // Antonius Michael Yordanis Hartono
    {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x10},  // Dinda Sofi Azzahro
    {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x11},  // Muhammad Fahmi Ilmi
    {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x10},  // Dhanishara Zaschya Putri Syamsudin
    {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x11},  // Irsa Fairuza
    {0x24, 0x0A, 0xC4, 0x0A, 0x14, 0x10},  // Revalinda Bunga Nayla Laksono
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x11},  // BISMA
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x10},  // JSON
    {0x24, 0x0A, 0xC4, 0x0A, 0x20, 0x11},  // FARUG
};

const char* mac_names[MAC_ADDRESS_TOTAL] = {
    "Fauzan Firdaus",                      // 0
    "Africha Sekar Wangi",                 // 1
    "Rafaina Erin Sadia",                  // 2
    "Antonius Michael Yordanis Hartono",   // 3
    "Dinda Sofi Azzahro",                  // 4
    "Muhammad Fahmi Ilmi",                 // 5
    "Dhanishara Zaschya Putri Syamsudin",  // 6
    "Irsa Fairuza",                        // 7
    "Revalinda Bunga Nayla Laksono",       // 8
    "BISMA",                               // 9
    "JSON",                                // 10
    "FARUG",                               // 11
};

typedef enum {
    FAUZAN_FIRDAUS = 0,
    AFRICHA_SEKAR_WANGI,
    RAFAINA_ERIN_SADIA,
    ANTONIUS_MICHAEL_YORDANIS_HARTONO,
    DINDA_SOFI_AZZAHRO,
    MUHAMMAD_FAHMI_ILMI,
    DHANISHARA_ZASCHYA_PUTRI_SYAMSUDIN,
    IRSA_FAIRUZA,
    REVALINDA_BUNGA_NAYLA_LAKSONO,
    BISMA,
    JSON,
    FARUG,
} ADDRESS_ASSIGNMENT;

ADDRESS_ASSIGNMENT MY_ADDRESS = IRSA_FAIRUZA;

// Fungsi yang bakal otomatis berjalan setiap kali ada data ESP-NOW masuk
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
    // INI NAMPILIN HURUF (W)
    Serial.println((char)incomingData[0]); 
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n[RECEIVER ESP-NOW]");
    // Set mode wifi
    WiFi.mode(WIFI_STA);

    // Set MAC ADDRES ku
    if (esp_wifi_set_mac(WIFI_IF_STA, mac_addresses[MY_ADDRESS]) != ESP_OK) {
        Serial.println("Error mengubah MAC address");
        return;
    }
    Serial.print("MAC Address Receiver di-set ke: ");
    Serial.println(mac_names[MY_ADDRESS]);
    Serial.print("Alamat MAC fisik: ");
    Serial.println(WiFi.macAddress());

    //inisialisasi esp now
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error inisialisasi ESP-NOW");
        return;
    }
    Serial.println("ESP-NOW berhasil di-init.");

    // DAFTARKAN FUNGSI "KOTAK SURAT" (CALLBACK)
    // "Tolong, kalau ada data masuk, panggil fungsi OnDataRecv"
    esp_now_register_recv_cb(OnDataRecv);
    Serial.println("Receiver siap menerima data...");
}

void loop() {
    // Tidak perlu melakukan apa-apa di loop
    // ESP-NOW bekerja di latar belakang (via interrupt)
    delay(1000);
}