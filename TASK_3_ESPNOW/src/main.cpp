#include "main.h"
//mengimpor semua deklarasi yg di main.h

const int mac_index_ku = 10;
//deklarasiin identitas aku ada di posisi 10

void setup() { 
     Serial.begin(115200); 
     //untuk memulai komunikasi
     Serial.println("Menyalakan ESP-NOW"); 
     mulai_esp_now(mac_index_ku); 
     //ini instalasi ESP NOW nya dan ngeset address ESP ku
     //setup dijalankan sekali saat ESP nyala untuk konfigurasi

  
     // TODO 1: Cetak nama mu gunakan fungsi mac_index_to_names 
    Serial.print("Namaku: ");
    Serial.println(mac_index_to_names(mac_index_ku));
    //ini ngambil nama dari index mac_index_ku

    Serial.println("Menunggu perintah..."); 
    //Tanda ESP udh siap
 } 

void loop() {
    if (Serial.available()) {
        baca_serial(callback_data_serial);
    }
    //ini akan mengecek apakah ada data di serial monitor
    //kalo ada, bakal dibaca sama fungsi baca_serial
    //terus call_back_data_serial bakal memproses
}