# DOKUMENTASI TASK 4 INTERN VI ROSE
Ini adalah proyek agar kita bisa mengirim data berupa .json dari laptop (serial monitor) ke ESP32 menggunakan kabel USB dan serial communication dan diteruskan ke ESP lain melalui ESP-NOW yang menggunakan SPIFFS.

## Alur Proyek ini
Laptop (Data Sender)
↓ (Serial)
ESP Bridge
↓ (ESP-NOW)
ESP Receiver (disimpan sementara di SPIFFS)

## Struktur Folder
Task4_ESPNOW/
│
├── data_sender/
│   ├── build/
│   ├── serial/
│   ├── rapidjson/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── biodata.json
│
├── esp_bridge/
│   ├── .pio/
│   ├── .vscode/
│   ├── include/
│   ├── lib/
│   ├── src/
│   │    └── main.cpp
│   ├── test/
│   ├── .gitignore
│   └── platform.ini
│
└── esp_receiver/
    ├── .pio/
    ├── .vscode/
    ├── include/
    ├── lib/
    ├── src/
    │    └── main.cpp
    ├── test/
    ├── .gitignore
    └── platform.ini

## Cara Kerja Proyek
1. Laptop membaca file **`biodata.json`**.
   Program di laptop baca file JSON
   **Baca file JSON**
     
   ```cpp
   ifstream ifs("biodata.json");
   Document doc;
   doc.ParseStream(IStreamWrapper(ifs));
   ```
   Isi data.json adalah

   ```json
   {
   "nama": "Snow White",
    "jurusan": "Desain Interior",
    "umur": 19,
    "deskripsi": "Aku adalah orang yang teliti dan suka menciptakan suasana nyaman. Hobiku merancang dekorasi ruangan dan mencari inspirasi desain dari majalah serta internet setiap hari agar sekelilingku jadi indah."
   }
   ```
3. Data JSON dipecah menjadi beberapa bagian karena memory ESP cuma bisa menampung 250 byte (chunk).
   ```cpp
   vector<string> chunks = splitString(jsonString, 50);
   ```
4. Tiap chunk dikirim ke **ESP Bridge** lewat port serial.
   - Abis data dipecah, setiap potongan dikirim satu per satu lewat serial communication ke ESP Bridge.
   - Ini dilakukan dengan jeda 200 ms biar ESP sempat membaca tiap potongan.
    ```cpp
    ser.write(chunks[i]);
    this_thread::sleep_for(chrono::milliseconds(200));
    ```

5. **ESP Bridge** nerusin data ke **ESP Receiver** lewat **ESP-NOW**.
   ESP Bridge bertugas menampung setiap chunk, lalu meneruskan datanya ke ESP Receiver lewat jalur ESP-NOW
   ```cpp
   void loop() {
   if (Serial.available()) {
    String data = Serial.readStringUntil('\n'); // Baca potongan data
    Serial2.println(data);  // Kirim ke ESP Receiver
    Serial.println("Data diteruskan: " + data);
   }
   }
   ```
6. **ESP Receiver** nyusun lagi datanya yang tadi dipisah-pisah, lalu disimpan di SPIFFS, dan ditampilin hasil lengkap di serial monitor.
   - Gabung potongan jadi satu JSON utuh, kalau data udah lengkap ketemu "}" di akhir
   ```cpp
   if (fullData.endsWith("}")) {
    // JSON lengkap
   }
   ```
   - Simpan ke SPIFFS supaya datanya gak hilang
   ```cpp
   File file = SPIFFS.open("/data.json", FILE_WRITE);
   file.print(fullData);
   file.close();
   ```
   - Tampilkan hasil di Serial Monitor
   ```cpp
     Serial.println("[KONTEN FILE YANG DITERIMA]");
     Serial.println("NAMA: " + String((const char*)doc["nama"]));
     ```
  ## Output Akhir di Serial Monitor Receiver
  ```yaml
NAMA: Snow White
JURUSAN: Desain Interior
UMUR: 19
DESKRIPSI DIRI: Aku adalah orang yang teliti dan suka menciptakan suasana nyaman. Hobiku merancang dekorasi ruangan dan mencari inspirasi desain dari majalah serta internet setiap hari agar sekelilingku jadi indah.
```
