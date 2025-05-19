cmake_minimum_required(VERSION 3.10)
project(HuffmanCompressorCpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(huffman-compressor-cpp
    src/main.cpp
    src/compressor.cpp
)