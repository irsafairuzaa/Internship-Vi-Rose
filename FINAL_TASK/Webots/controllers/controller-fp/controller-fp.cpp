// Template FP-1
// Referensi model robot: https://webots.cloud/run?version=R2025a&url=https%3A%2F%2Fgithub.com%2Fcyberbotics%2Fwebots%2Fblob%2Freleased%2Fprojects%2Frobots%2Frobotis%2Fdarwin-op%2Fprotos%2FRobotisOp2.proto
// Referensi Api Function: https://cyberbotics.com/doc/reference/nodes-and-api-functions

#include <iostream>
#include <map>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Robot.hpp>

#include <sstream>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <serial/serial.h>

namespace wb = webots;
namespace rj = rapidjson;
serial::Serial my_serial; //penghubung antara receiver dan webots

// Variabel untuk nyimpen pose-pose yang bergerak
rj::Document pose_jalan_maju;
rj::Document pose_jalan_mundur;
rj::Document belok_kanan;
rj::Document belok_kiri;
rj::Document geser_kanan;
rj::Document geser_kiri;

int current_frame = 0; // Untuk melacak frame animasi

const char* motorNames[] = {
        // Head motors
        "Head", "Neck",

        "ShoulderL", "ShoulderR",
        "ArmUpperL", "ArmUpperR",
        "ArmLowerL", "ArmLowerR",

        // Paha Z(Belok)
        "PelvYL", "PelvYR",

        // Paha X(Miring)
        "PelvL", "PelvR",

        // Paha atas Y(gerak depan / belakang)
        "LegUpperL", "LegUpperR",

        // Lutut
        "LegLowerL", "LegLowerR",

        // Tumit
        "AnkleL", "AnkleR",

        // Tumit X(miring)
        "FootL", "FootR"};

    // Initialize all motors and position sensors
    int numMotors = sizeof(motorNames) / sizeof(motorNames[0]);

//BACA file JSON isi variabel
void loadPoseFile(rj::Document& doc, const std::string& posePath) {
    if (doc.IsNull()) { 
        std::ifstream f(posePath);
        if (!f.is_open()) { /*... error ...*/ return; }
        rj::IStreamWrapper isw(f);
        doc.ParseStream(isw);
        f.close();
        if (doc.HasParseError()) { /*... error ...*/ }
        else { std::cout << "[SUCCESS] Pose '" << posePath << "' di-load." << std::endl; }
    }
}

// NERAPIN POSE dari Array Angka
void applyPoseFromArray(std::map<std::string, wb::Motor*>& motors, rj::Value& posisiArray) {
    if (posisiArray.Size() != numMotors) {
        std::cerr << "[ERROR] Jumlah motor di JSON tidak cocok!" << std::endl;
        return;
    }

// Loop dari 0 sampai 19
    for (int i = 0; i < numMotors; i++) {
        // Ambil NAMA motor dari array C++ kita
        const char* motorName = motorNames[i];
        
        // Ambil POSISI motor dari array JSON
        double position = posisiArray[i].GetDouble();

        // Terapkan
        if (motors.count(motorName)) {
            motors[motorName]->setPosition(position);
        }
    }
}

// Variabel buat lacak animasi
int group_index = 0;
int frame_index = 0;

int main(int argc, char** argv) {
    wb::Robot* robot = new wb::Robot();
    const int timeStep = static_cast<int>(robot->getBasicTimeStep());

    // Map to store all motors
    std::map<std::string, wb::Motor*> motors;
    std::map<std::string, wb::PositionSensor*> positionSensors;

    
    std::cout << "Initializing " << numMotors << " motors..." << std::endl;

    for (int i = 0; i < numMotors; i++) {
        // Get each motor
        if (motor) {
            motors[motorNames[i]] = motor;
            std::cout << "[SUCCESS] Motor found: " << motorNames[i] << std::endl;

            // Get corresponding position sensor
            std::string sensorName = std::string(motorNames[i]) + "S";
            wb::PositionSensor* sensor = robot->getPositionSensor(sensorName);
            if (sensor) {
                sensor->enable(timeStep);
                positionSensors[motorNames[i]] = sensor;
                std::cout << "[SUCCESS] Sensor enabled! : " << sensorName << std::endl;
            }
        } else {
            std::cerr << "[FAILED] Motor not found! : " << motorNames[i] << std::endl;
        }
    }

    //Load file berdiri
    rj::Document pose_berdiri;
    loadPoseFile(pose_berdiri, "../../poses/pose-berdiri.json");
    
    // Terapkan pose berdiri sebagai pose awal
    std::cout << "\nSetting initial 'berdiri' pose..." << std::endl;
    if (!pose_berdiri.IsNull()) {
        applyPoseFromArray(motors, pose_berdiri["posisi"]);
    }

    while (robot->step(timeStep) != -1) {
        
        char key = 0;
        if (my_serial.available() > 0) { /*... baca serial ...*/ }

        //logika gerakan
        if (key == 'W') {
            if (!pose_jalan_maju.IsNull()) {
                // Ambil 'posisi' array dari struktur JSON
                rj::Value& posisi = pose_jalan_maju["pose_group"][group_index]["pose"][frame_index]["posisi"];
                
                // Terapkan ke motor
                applyPoseFromArray(motors, posisi);

                // Majukan frame animasi
                frame_index++;
                // Jika frame dalam 1 group habis (ada 4 frame)
                if (frame_index >= pose_jalan_maju["pose_group"][group_index]["pose"].Size()) {
                    frame_index = 0; // Reset frame
                    group_index++; // Pindah ke group berikutnya
                    
                    // Jika group habis (ada 6 group)
                    if (group_index >= pose_jalan_maju["pose_group"].Size()) {
                        group_index = 0; // Ulangi dari group pertama
                    }
                }
            }
        }

        else if (key == 'S') {
            if (!pose_jalan_mundur.IsNull()) {
                rj::Value& posisi = pose_jalan_mundur["pose_group"][group_index]["pose"][frame_index]["posisi"];
                applyPoseFromArray(motors, posisi);

                frame_index++;
                if (frame_index >= pose_jalan_mundur["pose_group"][group_index]["pose"].Size()) {
                    frame_index = 0;
                    group_index++;
                    if (group_index >= pose_jalan_mundur["pose_group"].Size()) {
                        group_index = 0;
                    }
                }
            }
        }
        
        else {
            // Jika tidak ada tombol, kembali ke pose berdiri
            if (!pose_berdiri.IsNull()) {
                applyPoseFromArray(motors, pose_berdiri["posisi"]);
            }
            // Reset animasi
            frame_index = 0;
            group_index = 0;
        }
    }

    my_serial.close();
    delete robot;
    return 0;

}
