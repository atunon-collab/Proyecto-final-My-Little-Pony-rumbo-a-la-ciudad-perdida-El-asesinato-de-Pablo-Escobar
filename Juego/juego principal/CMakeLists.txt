cmake_minimum_required(VERSION 3.19)
project(CiudadPerdida LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)
qt_standard_project_setup()

# El codigo esta en esta carpeta ("juego principal")
file(GLOB FUENTES *.cpp *.h)
qt_add_executable(CiudadPerdida WIN32 ${FUENTES})
target_link_libraries(CiudadPerdida PRIVATE Qt6::Widgets Qt6::Multimedia)

# Imagenes y sonidos de la carpeta recursos, dentro del ejecutable
file(GLOB_RECURSE RECURSOS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ../recursos/sprites/*.png ../recursos/fondos/*.png ../recursos/objetos/*.png
    ../recursos/sonidos/*.wav ../recursos/sonidos/*.mp3 ../recursos/sonidos/*.ogg ../recursos/fuentes/*.ttf)
qt_add_resources(CiudadPerdida "recursos" PREFIX "/" BASE "../recursos" BIG_RESOURCES FILES ${RECURSOS})
