#include <Arduino.h>
#include <EEPROM.h> //agar nilainya ga akan hilang, kyk memori

#define ESP_PIN 2
void setup() {
  Serial.begin(115200); 
  //Buat dia berkomunikasi sama si alatnya
  Serial.println("Hello, world!");
  //hello world cuma bisa di output dan dijalankan sekali selama dicolok
  EEPROM.begin(128); //128 bytes
  //128 bytes itu kapasitasnya
  String nama = "Irsa";
  for(int i=0; i<nama.length(); i++){
    EEPROM.writeByte(i, nama[i]);
  }

  //pin" di alatnya itu buat komunikasi antar ESP
  pinMode(LED_PIN, OUTPUT);
  pinMode(0, INPUT_PULLUP); 
  //kalau pullup itu kondisi pin dipaksa HIGH, dan kalau tombol dipencet dia jadi LOW
}

void loop(){
  //nyala matiin LED
  digitalWrite(LED_PIN, HIGH);
  serial.println(digitalRead(LED_PIN));
  //digital read itu buat abis baca pin, dia mengambalikan nilai boolean

//mau baca kondisi tombolnya dipencet atau ngga pake if
  if(digitalRead(0)){
    Serial.println("Tombol dipencet");
  }else{
    Serial.println("Tombol tidak dipencet");
  }
}