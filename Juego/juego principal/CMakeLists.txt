cmake_minimum_required(VERSION 3.19)
project(CiudadPerdida LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)
qt_standard_project_setup()

file(GLOB FUENTES *.cpp *.h)
qt_add_executable(CiudadPerdida WIN32 ${FUENTES})
target_link_libraries(CiudadPerdida PRIVATE Qt6::Widgets Qt6::Multimedia)

