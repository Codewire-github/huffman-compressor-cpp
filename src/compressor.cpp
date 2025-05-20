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

void serializeTree(HuffmanNode* node, std::ostream& out) {
    if (!node) return;

    if (node->isLeaf()) {
        out.put('1');   // Marker for the leaf
        out.put(static_cast<char>(node->byte));
    }
    else {
        out.put('0');   // Marker for the internal node
        serializeTree(node->left, out);
        serializeTree(node->right, out);
    }
}

void encodeAndWriteCompressed(
    const std::string& inputPath,
    const std::map<uint8_t, std::string>& huffmandCodes,
    HuffmanNode* treeRoot,
    const std::string& outputPath
) {
    std::ifstream input(inputPath, std::ios::binary);
    if (!input) throw std::runtime_error("Cannot open input file for encoding");

    std::ofstream output(outputPath, std::ios::binary);
    if (!output) throw std::runtime_error("Cannot open output file for writing");

    // Serializing Huffman tree
    serializeTree(treeRoot, output);

    // Building bit buffer
    std::string bitBuffer;
    char ch;
    while (input.get(ch)) {
        uint8_t byte = static_cast<uint8_t>(ch);
        bitBuffer += huffmandCodes.at(byte);
    }

    // Writing number of meaningful bits
    uint32_t totalBits = static_cast<uint32_t>(bitBuffer.size());
    output.write(reinterpret_cast<const char*>(&totalBits), sizeof(totalBits));  // MUST come BEFORE writing bitstream

    // Writing compressed bitstream
    while (bitBuffer.size() >= 8) {
        std::bitset<8> byteBits(bitBuffer.substr(0, 8));
        bitBuffer.erase(0, 8);
        output.put(static_cast<uint8_t>(byteBits.to_ulong()));
    }

    if (!bitBuffer.empty()) {
        while (bitBuffer.size() < 8) bitBuffer += '0'; // pad to full byte
        std::bitset<8> lastBits(bitBuffer);
        output.put(static_cast<uint8_t>(lastBits.to_ulong()));
    }

    input.close();
    output.close();
}



HuffmanNode* deserializeTree(std::istream& in) {
    char marker;
    if (!in.get(marker)) throw std::runtime_error("Invalid or corrupted compressed file");

    if (marker == '1') {
        char byteChar;
        if (!in.get(byteChar)) throw std::runtime_error("Unexpected EOF during tree deserialization");
        return new HuffmanNode(static_cast<uint8_t>(byteChar), 0);
    }
    else if (marker == '0') {
        HuffmanNode* left = deserializeTree(in);
        HuffmanNode* right = deserializeTree(in);
        return new HuffmanNode( 0, left, right);
    }
    else {
        throw std::runtime_error("Invalid tree marker found during the deserialization");
    }
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
        encodeAndWriteCompressed(inputFile, huffmanCodes, root, outputFile);

        std::cout << "Compresion completed: " << outputFile << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Compression Failed: " << ex.what() << std::endl;
    }
}

void Compressor::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile, std::ios::binary);
    if (!input) throw std::runtime_error("Cannot open compressed file");

    HuffmanNode* root = deserializeTree(input);
    HuffmanNode* current = root;

    uint32_t totalBits = 0;
    input.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));

    std::ofstream output(outputFile, std::ios::binary);
    if (!output) throw std::runtime_error("Cannot open output file");

    uint32_t bitsRead = 0;
    char ch;

    while (input.get(ch) && bitsRead < totalBits) {
        std::bitset<8> bits(static_cast<uint8_t>(ch));
        for (int i = 7; i >= 0 && bitsRead < totalBits; --i, ++bitsRead) {
            current = bits[i] ? current->right : current->left;
            if (current->isLeaf()) {
                output.put(static_cast<char>(current->byte));
                current = root;
            }
        }
    }

    input.close();
    output.close();

}

