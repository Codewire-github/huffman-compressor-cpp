#pragma once
#include <string>

struct HuffmanNode {
    uint8_t byte;                   // Byte value (Only for leaf node)
    size_t frequency;               // Frequency of the byte or sum of children bytes
    HuffmanNode* left = nullptr;    // Left child of node
    HuffmanNode* right = nullptr;   // Right child of node

    HuffmanNode(uint8_t b, size_t freq) : byte(b), frequency(freq) {}
    HuffmanNode(size_t freq, HuffmanNode* l, HuffmanNode* r) : frequency(freq), left(l), right(r) {}

    // Checking if the node is a leaf
    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

class Compressor {
public:
    void compress(const std::string& inputFile, const std::string& outputFile);
    void decompress(const std::string& inputFile, const std::string& outputFile);
};