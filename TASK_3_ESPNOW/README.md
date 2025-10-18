# DOKUMENTASI TASK 3 ESP-NOW SAPA MENYAPA

Proyek ini adalah proyek task 3 pada internship robotik vi rose, ini merupakan implementasi dari komunikasi antar ESP32 menggunakan ESP-NOW. Implementasi ini memungkinkan untuk komunikasi antar ESP dimana masing-masing ESP dapat melakukan komunikasi seperti:
1. Mengirim pesan HALO untuk sapaan antar ESP
2. Mengecek status antar ESP dengan perintah CEK
3. Menampilkan jawaban dari ESP tujuan dengan JAWAB

## Struktur folder src
```
├──main.cpp
├──main.h
└──utility.cpp
```
- main.cpp >> sebagai setup atau konfigurasi ESP-NOW untuk menjalankan program
- main.h >> header file yg menyimpan deklarasi fungsi, enum, array MAC
- utility.cpp >> berisi logika & fungsi pendukung (baca_serial, proses_perintah, mac_index_to_name, dll)

## Deskripsi masing-masing file
### 1) main.cpp - Awal dari pemrograman

- File utama yang dijalankan saat ESP nyala
- Menyiapkan komunikasi Serial dan ESP-NOW
- Menentukan identitas ESP dan mulai loop

```cpp
void setup() { 
     Serial.begin(115200); 
     //untuk memulai komunikasi
     Serial.println("Menyalakan ESP-NOW"); 
     mulai_esp_now(mac_index_ku); 
     //ini instalasi ESP NOW dan set address ESP ku
```
Ini merupakan bagian untuk memulai komunikasi lalu melakukan instalasi ESP nya dan ngeset address ESP ku. Setup ini dijalankan sekali saat ESP nyala, buat konfigurasi.

```cpp
 // TODO 1: Cetak nama mu gunakan fungsi mac_index_to_names 
    Serial.print("Halo besti aku ");
    Serial.println(mac_index_to_names(mac_index_ku));

    Serial.println("Menunggu perintah..."); 
    //Tanda ESP udh siap
 }
```
Pada TODO 1 ini kita mengambil alamat MAC yang ada di array mac_addresses pada file main.h untuk identitas ESP kita. Jika sudah ditentukan kita menggunakan fungsi mac_index_to_names(int mac_index) untuk konversikan index ke nama ESP (disini nama ESP nya Irsa Fairuza). Jika sudah napa ESP akan ditampilkan di Serial Monitor.

```cpp
void loop() {
    if (Serial.available()) {
        baca_serial(callback_data_serial);
    }
}
```
Disini akan dicek apakah ada data di serial monitor, kalau ada akan dibaca oleh fungsi baca_serial dan akan diproses.

### 2) main.h - Header Deklarasi
File header ini berisi:
- Include Library
- Konstanta (ukuran array, jumlah ESP, dan buffer data)
- Enum identitas dan perintah
- Variabel global

### 3) utility.cpp - Logika Komunikasi ESP
- Menyimpan semua logika dan fungsi bantu
- Inisialisasi jaringan ESP-NOW
- Menyimpan daftar MAC dan nama ESP
- Menerima dan memproses data dari ESP atau Serial
- Mengirim balasan otomatis sesuai perintah

#### Fungsi baca_serial (TODO 2)
```cpp
void baca_serial(void (*callback)(const uint8_t *data, int len)) {
    static uint8_t paket[BUFFER_SIZE];
    static int tahap = 0;
    static int panjang = 0;
    static int posisi = 0;

    while (Serial.available()) {
        uint8_t masuk = Serial.read();

        switch (tahap) {
            case 0: 
                if (masuk == 0xFF) tahap = 1;
                break;

            case 1:
                if (masuk == 0xFF) tahap = 2;
                else tahap = 0;
                break;

            case 2:
                if (masuk == 0x00) tahap = 3;
                break;

            case 3:  // Ambil panjang data
                panjang = masuk;
                if (panjang > 0 && panjang <= BUFFER_SIZE) {
                    posisi = 0;
                    tahap = 4;
                } else {
                    tahap = 0;  // reset kalau panjang ga valid
                }
                break;

            case 4:  // Simpan data ke paket
                paket[1 + posisi] = masuk;
                posisi++;

                if (posisi == panjang) {
                    callback(paket, 1 + panjang);
                    tahap = 0;
                }
                break;
        }
    }
}
```

#### Fungsi process_perintah (TODO 3)
```cpp
void process_perintah(const uint8_t *data, int len, int index_mac_address_asal) {
    // TODO 3: implementasi kode buat processing perintah
    if (len < 2) return;

    uint8_t kode = data[0];               // jenis aksi: 0x00 = HALO, 0x01 = CEK, 0x02 = JAWAB
    uint8_t index_tujuan = data[1];       // tujuan MAC

    String nama_asal = (index_mac_address_asal == -1) ? "Laptop" : mac_index_to_names(index_mac_address_asal);
    String nama_tujuan = (index_tujuan >= 0 && index_tujuan < MAC_ADDRESS_TOTAL) ? mac_names[index_tujuan] : "Unknown";
    String namaku = mac_index_to_names(mac_index_ku);

    switch (kode) {
        case 0x00:  // HALO
            if (index_mac_address_asal == -1) {
                String pesan = "Hai hai " + nama_tujuan + ", aku " + namaku;
                uint8_t kirim[1 + pesan.length()];
                kirim[0] = 0x00;
                memcpy(&kirim[1], pesan.c_str(), pesan.length());
                esp_now_send(mac_addresses[index_tujuan], kirim, sizeof(kirim));
                Serial.println("Mengirim HALO ke " + nama_tujuan);
            } else {
                // Balasan HALO dari ESP
                String balasan = "Halo jugaa " + nama_asal + ", aku " + namaku;
                uint8_t kirim[1 + balasan.length()];
                kirim[0] = 0x02;
                memcpy(&kirim[1], balasan.c_str(), balasan.length());
                esp_now_send(mac_addresses[index_mac_address_asal], kirim, sizeof(kirim));
            }
            break;

        case 0x01:  // CEK
            if (index_mac_address_asal == -1) {
                // Kirim CEK dari Serial
                String pesan ="Heyy" + nama_tujuan + ", ini " + namaku + ". Kamu ada ga di sana?";
                uint8_t kirim[1 + pesan.length()];
                kirim[0] = 0x01;
                memcpy(&kirim[1], pesan.c_str(), pesan.length());
                esp_now_send(mac_addresses[index_tujuan], kirim, sizeof(kirim));
                Serial.println("Mengirim CEK ke " + nama_tujuan);
            } else {
                // Balasan CEK dari ESP
                String balasan = "Iyess " + nama_asal + ", aku " + namaku + " ada kokk";
                uint8_t kirim[1 + balasan.length()];
                kirim[0] = 0x02;
                memcpy(&kirim[1], balasan.c_str(), balasan.length());
                esp_now_send(mac_addresses[index_mac_address_asal], kirim, sizeof(kirim));
            }
            break;

        case 0x02:  // JAWAB
        if (len > 1){
        String pesan = String((char *)&data[1]);
        Serial.println("Balasan dari " + nama_asal + ": " + pesan);
        } else {
        Serial.println("JAWAB kosong dari " + nama_asal);  
        }
            break;

        default:
            Serial.println("Kode perintah tidak dikenali: " + String(kode));
            break;
    }
}
```
