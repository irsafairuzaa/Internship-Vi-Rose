#include <iostream>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

using namespace std;
namespace rj = rapidjson;

int main()
{
    int movie;
    cout << "Masukkan motion_movie: ";
    cin >> movie;

    string path_json = "XL/motion_bucket/motion_unit";
    ifstream input_file(path_json);
    if (!input_file.is_open()) 
    {
        cout << "gagal membuka file" << std::endl;
        return 1;
    }

    rj::IStreamWrapper isw(input_file);
    rj::Document doc;
    doc.ParseStream(isw);
    if (doc.HasParseError())
    {
        cout << "JSON parse gagal" << std::endl;
    }


if (doc.HasMember("motion_movie_id") && doc["motion_movie_id"].IsInt()) {
    int movie_id = doc["motion_movie_id"].GetInt();
    if (movie_id == movie) {
            const rj::Value& unit_id = doc["ID"];
            cout << "Motion Unit ID: " << unit_id.GetInt() << endl;
        }
    
    if (doc.HasMember("motion_frame") && doc["motion_frame"].IsArray()) {
    const Value& frames = doc["motion_frame"];
    for (const Value& frame : frames.GetArray()) {
        cout << "Motion Frame: [";
        for (rj::SizeType i = 0; i < frame.Size(); ++i) {
            cout << frame[i].GetInt();
            if (i < frame.Size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }