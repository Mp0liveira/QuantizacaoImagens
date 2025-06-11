cmake_minimum_required(VERSION 3.29)
project(Quantizacao)

set(CMAKE_CXX_STANDARD 20)

add_executable(Quantizacao main.cpp
        includes/QuantizadorEscalar.h
        src/QuantizadorEscalar.cpp)

target_include_directories(Quantizacao PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs)
