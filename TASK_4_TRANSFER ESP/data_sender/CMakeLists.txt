cmake_minimum_required(VERSION 3.15)

# Project Name
project("Task4ESPNOW")

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rapidjson_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include/")
set(serial_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/serial/include/")
set(serial_src_dir "${CMAKE_CURRENT_SOURCE_DIR}/serial/src")


include_directories(${rapidjson_include_dir})
include_directories(${serial_include_dir})

# include_directories()

add_executable(data_sender main.cpp ${serial_src_dir}/serial.cc)

# target_link_libraries()