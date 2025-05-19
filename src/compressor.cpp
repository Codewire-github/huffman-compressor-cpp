#include "compressor.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <queue>
#include <memory>


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

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency; // Min-heap (lowest frequency first)
    }
};

HuffmanNode* buildHuffmanTree(const std::unordered_map<uint8_t, size_t>& freqTable) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode* >, Compare> minHeap;

    // Creating leaf nodes and pushing it to min-heap
    for (const auto& [byte, freq] : freqTable) {
        minHeap.push(new HuffmanNode(byte, freq));
    }

    // Combining all the nodes until one remains
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        size_t combinedFreq = left->frequency + right->frequency;
        HuffmanNode* parent = new HuffmanNode(combinedFreq, left, right);

        minHeap.push(parent);
    }

    // Final node is the root of the tree
    return minHeap.top();
}

void Compressor::compress(const std::string& inputFile, const std::string& outputFile) {
    std::cout << "Compressing: " << inputFile << " -> " << outputFile << std::endl;
    try {
        auto freqTable = buildFrequencyTable(inputFile);
        std::cout << "Byte frequencies:\n";
        for (const auto& [byte, freq] : freqTable) {
            std::cout << "Byte " << static_cast<int>(byte) << " occurred " << freq << " times\n";
        }

        HuffmanNode* root = buildHuffmanTree(freqTable);
        std::cout << "Huffman Tree built successfully\n";

        // TODO: Encode the tree
    }
    catch (const std::exception& ex) {
        std::cerr << "Compression Failed: " << ex.what() << std::endl;
    }
}

void Compressor::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::cout << "Decompressing: " << inputFile << " -> " << outputFile << std::endl;
    // TODO: Huffman decompression
}

