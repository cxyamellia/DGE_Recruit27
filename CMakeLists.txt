cmake_minimum_required(VERSION 3.16)
project(DGE_Recruit27)

find_package(OpenCV REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

add_subdirectory(io)
add_subdirectory(color_correct)
add_subdirectory(armor_detect)
