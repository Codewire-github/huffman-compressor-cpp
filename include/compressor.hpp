#pragma once
#include <string>

class Compressor {
public:
    void compress(const std::string& inputFile, const std::string& outputFile);
    void decompress(const std::string& inputFile, const std::string& outputFile);
};