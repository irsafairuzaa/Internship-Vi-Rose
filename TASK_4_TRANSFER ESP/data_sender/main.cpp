#include <iostream>        
#include <fstream>         
#include <string>           
#include <vector>                   
#include <serial/serial.h> 
#include <rapidjson/document.h>       
#include <rapidjson/istreamwrapper.h> 

using namespace std;
using namespace rapidjson;

//Fungsi buat mecah string
vector<string> splitString(const string &biodata, size_t chunkSize) {
    vector<string> chunks;
    for (size_t i = 0; i < biodata.size(); i += chunkSize) {
        chunks.push_back(biodata.substr(i, chunkSize));
    }
    return chunks;
}

int main() {

    //Buka file JSON
    ifstream ifs("biodata.json");
    if (!ifs.is_open()) {   
        cout << "Gagal membuka file data.json" << endl;
        return 1;
    }

    //Bungkus file stream ke RapidJSON IStreamWrapper
    IStreamWrapper isw(ifs);

    //Parse isi JSON
    Document doc;
    doc.ParseStream(isw);

    //Cek kalau parsing gagal
    if (doc.HasParseError()) {
        cerr << "Gagal parsing file JSON!" << endl;
        return 1;
    }

    //Ambil data dari JSON dan ubah jadi 1 string utuh
    string jsonString;
    jsonString += "{";
    jsonString += "\"nama\":\"" + string(doc["nama"].GetString()) + "\",";
    jsonString += "\"jurusan\":\"" + string(doc["jurusan"].GetString()) + "\",";
    jsonString += "\"umur\":" + to_string(doc["umur"].GetInt()) + ",";
    jsonString += "\"deskripsi\":\"" + string(doc["deskripsi"].GetString()) + "\"";
    jsonString += "}";

    cout << "\nIsi JSON yang akan dikirim:\n" << jsonString << endl;

    //Pecah data JSON jadi potongan (misal 50 byte tiap potongan)
    vector<string> chunks = splitString(jsonString, 50);

    cout << "\nTotal potongan data: " << chunks.size() << endl;

    //Hubungkan ke port serial ESP32
    string port = "/dev/ttyUSB0";
    unsigned long baudrate = 115200;

    try {
        serial::Serial ser(port, baudrate, serial::Timeout::simpleTimeout(1000));

        if (!ser.isOpen()) {
            cerr << "Port serial gagal dibuka!" << endl;
            return 1;
        }

        cout << "\n Port serial berhasil dibuka di " << port << endl;

        //Kirim setiap potongan ke ESP satu per satu
        for (size_t i = 0; i < chunks.size(); ++i) {
            string dataToSend = chunks[i];

            // Kirim data lewat serial
            ser.write(dataToSend);

            cout << "Mengirim potongan ke-" << i + 1 << ": " << dataToSend << endl;

        }

        cout << "\n Semua potongan sudah dikirim!" << endl;

        //Tutup port
        ser.close();
    }
}
