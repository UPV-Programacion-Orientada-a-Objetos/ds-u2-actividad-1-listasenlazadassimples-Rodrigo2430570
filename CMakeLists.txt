cmake_minimum_required(VERSION 3.15)
project(iot_sensores LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(iot_sensores
    src/main.cpp
    src/ListaSensor.h
    src/SensorBase.h
    src/SensorTemperatura.h
    src/SensorTemperatura.cpp
    src/SensorPresion.h
    src/SensorPresion.cpp
    src/ListaGeneral.h
    src/ListaGeneral.cpp
)

# On Windows, ensure console subsystem
if (MSVC)
    target_compile_options(iot_sensores PRIVATE /W4 /permissive-)
else()
    target_compile_options(iot_sensores PRIVATE -Wall -Wextra -pedantic)
endif()