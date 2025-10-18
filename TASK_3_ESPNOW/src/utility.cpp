#include "main.h"

esp_now_peer_info_t peer_info;
//variabel buat nyimpen data" peer ESP-NOW

uint8_t mac_addresses[MAC_ADDRESS_TOTAL][MAC_ADDRESS_LENGTH] = {
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x11},  // BISMA
    {0x24, 0x0A, 0xC4, 0x0A, 0x21, 0x10},  // JSON
    {0x24, 0x0A, 0xC4, 0x0A, 0x20, 0x11},  // FARUG
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x10},  // Fauzan Firdaus
    {0x24, 0x0A, 0xC4, 0x0A, 0x10, 0x11},  // Africha Sekar wangi
    {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x10},  // Rafaina Erin Sadia
    {0x24, 0x0A, 0xC4, 0x0A, 0x11, 0x11},  // Antonius Michael Yordanis Hartono
    {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x10},  // Dinda Sofi Azzahro
    {0x24, 0x0A, 0xC4, 0x0A, 0x12, 0x11},  // Muhammad Fahmi Ilmi
    {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x10},  // Dhanishara Zaschya Putri Syamsudin
    {0x24, 0x0A, 0xC4, 0x0A, 0x13, 0x11},  // Irsa Fairuza
    {0x24, 0x0A, 0xC4, 0x0A, 0x14, 0x10},  // Revalinda Bunga Nayla Laksono

};
//mac_address itu daftar MAC address semua ESP yang mau komunikasi

const char *mac_names[MAC_ADDRESS_TOTAL] = {
    "BISMA",                               // 0
    "JSON",                                // 1
    "FARUG",                               // 2
    "Fauzan Firdaus",                      // 3
    "Africha Sekar Wangi",                 // 4
    "Rafaina Erin Sadia",                  // 5
    "Antonius Michael Yordanis Hartono",   // 6
    "Dinda Sofi Azzahro",                  // 7
    "Muhammad Fahmi Ilmi",                 // 8
    "Dhanishara Zaschya Putri Syamsudin",  // 9
    "Irsa Fairuza",                        // 10
    "Revalinda Bunga Nayla Laksono",       // 11
};
//masukin nama sesuai index MAC yg udah ditentuin

esp_err_t mulai_esp_now(int index_mac_address)
//instalasi ESP NOW dan set MAC address
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    /* Init ESP-NOW */
    esp_err_t result = esp_now_init();
    if (result != ESP_OK)
        return result;

    /* Set callback function to handle received data */
    result = esp_now_register_recv_cb(callback_data_esp_now);
    if (result != ESP_OK)
        return result;

    result = esp_now_register_send_cb(callback_pengiriman_esp_now);
    //     if (result != ESP_OK)
    //         return result;

    /* Set MAC Address */
    uint8_t mac[MAC_ADDRESS_LENGTH];
    memcpy(mac, mac_addresses[index_mac_address], MAC_ADDRESS_LENGTH);
    result = esp_wifi_set_mac(WIFI_IF_STA, mac);
    if (result != ESP_OK)
        return result;

    /* Initialize peer_info and set fields*/
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
    peer_info.channel = 0;
    peer_info.encrypt = false;

    /* Add All MAC to peer list  */
    for (int i = 0; i < MAC_ADDRESS_TOTAL; i++) {
        memcpy(peer_info.peer_addr, mac_addresses[i], MAC_ADDRESS_LENGTH);
        result = esp_now_add_peer(&peer_info);
        if (result != ESP_OK)
            return result;
    }

    return ESP_OK;
}

int cari_mac_index(const uint8_t *mac) {
    for (int i = 0; i < MAC_ADDRESS_TOTAL; i++) {
        // Compare the MAC address
        if (memcmp(mac, mac_addresses[i], MAC_ADDRESS_LENGTH) == 0)
            return i;
    }

    // if not found return -1
    return -1;
}

String mac_index_to_names(int mac_index) {
    if ((mac_index < 0 || mac_index >= MAC_ADDRESS_TOTAL) || (mac_index == -1)) {
        return "Unknown";
    }
    return mac_names[mac_index];
}

void callback_data_esp_now(const uint8_t *mac, const uint8_t *data, int len) {
    int index_mac_asal = cari_mac_index(mac);
    process_perintah(data, len, index_mac_asal);
}
void callback_pengiriman_esp_now(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.printf("\nStatus pengiriman ESP-NOW: %s\n", esp_err_to_name(status));
}
void callback_data_serial(const uint8_t *data, int len) {
    process_perintah(data, len);
}
void baca_serial(void (*callback)(const uint8_t *data, int len)) {
    static uint8_t paket[BUFFER_SIZE];
    static int tahap = 0;
    static int panjang = 0;
    static int posisi = 0;

    while (Serial.available()) {
        uint8_t masuk = Serial.read();

        switch (tahap) {
            case 0:  // Tunggu FF pertama
                if (masuk == 0xFF) tahap = 1;
                break;

            case 1:  // Tunggu FF kedua
                if (masuk == 0xFF) tahap = 2;
                else tahap = 0;
                break;

            case 2:  // Ambil jenis perintah
                paket[0] = masuk;  // simpan command di paket[0]
                tahap = 3;
                break;

            case 3:  // Ambil panjang data
                panjang = masuk;
                if (panjang > 0 && panjang <= BUFFER_SIZE - 1) {
                    posisi = 0;
                    tahap = 4;
                } else {
                    tahap = 0;  // reset kalau panjang tidak valid
                }
                break;

            case 4:  // Simpan data ke paket
                paket[1 + posisi] = masuk;
                posisi++;

                if (posisi == panjang) {
                    callback(paket, 1 + panjang);  // kirim command + data
                    tahap = 0;
                }
                break;
        }
    }
}

void process_perintah(const uint8_t *data, int len, int index_mac_address_asal) {
    // TODO 3: implementasi kode buat processing perintah
void process_perintah(const uint8_t *data, int len, int index_mac_address_asal); {
    if (len < 2) return;

    uint8_t kode = data[0];               // jenis aksi: 0x00 = HALO, 0x01 = CEK, 0x02 = JAWAB
    uint8_t index_tujuan = data[1];       // tujuan MAC

    String nama_asal = (index_mac_address_asal == -1) ? "Laptop" : mac_index_to_names(index_mac_address_asal);
    String nama_tujuan = (index_tujuan >= 0 && index_tujuan < MAC_ADDRESS_TOTAL) ? mac_names[index_tujuan] : "Unknown";
    String namaku = mac_index_to_names(mac_index_ku);

    switch (kode) {
        case 0x00:  // HALO
            if (index_mac_address_asal == -1) {
                // Kirim HALO dari Serial
                String pesan = "Halo " + nama_tujuan + ", aku " + namaku;
                uint8_t kirim[1 + pesan.length()];
                kirim[0] = 0x00;
                memcpy(&kirim[1], pesan.c_str(), pesan.length());
                esp_now_send(mac_addresses[index_tujuan], kirim, sizeof(kirim));
                Serial.println("Mengirim HALO ke " + nama_tujuan);
            } else {
                // Balasan HALO dari ESP
                String balasan = "Halo juga " + nama_asal + ", aku " + namaku;
                uint8_t kirim[1 + balasan.length()];
                kirim[0] = 0x02;
                memcpy(&kirim[1], balasan.c_str(), balasan.length());
                esp_now_send(mac_addresses[index_mac_address_asal], kirim, sizeof(kirim));
            }
            break;

        case 0x01:  // CEK
            if (index_mac_address_asal == -1) {
                // Kirim CEK dari Serial
                String pesan = nama_tujuan + ", ini " + namaku + ". Kamu di sana?";
                uint8_t kirim[1 + pesan.length()];
                kirim[0] = 0x01;
                memcpy(&kirim[1], pesan.c_str(), pesan.length());
                esp_now_send(mac_addresses[index_tujuan], kirim, sizeof(kirim));
                Serial.println("Mengirim CEK ke " + nama_tujuan);
            } else {
                // Balasan CEK dari ESP
                String balasan = "Iya " + nama_asal + ", aku " + namaku + " di sini!";
                uint8_t kirim[1 + balasan.length()];
                kirim[0] = 0x02;
                memcpy(&kirim[1], balasan.c_str(), balasan.length());
                esp_now_send(mac_addresses[index_mac_address_asal], kirim, sizeof(kirim));
            }
            break;

        case 0x02:  // JAWAB
            if (len > 1) {
                String isi = "";
                for (int i = 1; i < len; i++) {
                    isi += (char)data[i];
                }
                Serial.println("Balasan dari " + nama_asal + ": " + isi);
            } else {
                Serial.println("JAWAB kosong dari " + nama_asal);
            }
            break;

        default:
            Serial.println("Kode perintah tidak dikenali: " + String(kode));
            break;
    }
}