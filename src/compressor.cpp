#include "compressor.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <queue>
#include <memory>
#include <string>
#include <map>


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

void generateHuffmanCodes(HuffmanNode* node, const std::string& path, std::map<uint8_t, std::string>& huffmanCodes) {
    if (!node) return;
    if (node->isLeaf()) {
        huffmanCodes[node->byte] = path;
        return;
    }

    generateHuffmanCodes(node->left, path + "0", huffmanCodes);
    generateHuffmanCodes(node->right, path + "1", huffmanCodes);
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

        std::map<uint8_t, std::string> huffmanCodes;
        generateHuffmanCodes(root, "", huffmanCodes);

        std::cout << "Huffman Codes:\n";
        for (const auto& [byte, code] : huffmanCodes) {
            std::cout << static_cast<int>(byte) << " -> " << code << '\n';
        }

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

