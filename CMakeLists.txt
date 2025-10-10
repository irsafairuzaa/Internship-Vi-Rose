cmake_minimum_required(VERSION 3.15)
project(MotionProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include)

add_executable(MotionProject main.cpp)
