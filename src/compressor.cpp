#include "compressor.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdint>


std::unordered_map<uint8_t, size_t> buildFrequencyTable(const std::string& filePath) {
    std::unordered_map<uint8_t, size_t> freqTable;

    std::ifstream input(filePath, std::ios::binary);

    if (!input) {
        throw std::runtime_error("Failed to open input file: " + filePath);
    }

    char byte;
    while (input.get(byte)) {
        uint8_t uByte = static_cast<uint8_t>(byte);
        freqTable[uByte]++;
    }

    input.close();
    return freqTable;
}

void Compressor::compress(const std::string& inputFile, const std::string& outputFile) {
    std::cout << "Compressing: " << inputFile << " -> " << outputFile << std::endl;
    try {
        auto freqTable = buildFrequencyTable(inputFile);
        std::cout << "Byte frequencies:\n";
        for (const auto& [byte, freq] : freqTable) {
            std::cout << "Byte " << static_cast<int>(byte) << " occurred " << freq << " times\n";
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Compression Failed: " << ex.what() << std::endl;
    }
}

void Compressor::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::cout << "Decompressing: " << inputFile << " -> " << outputFile << std::endl;
    // TODO: Huffman decompression
}

