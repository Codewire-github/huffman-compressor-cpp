#include "compressor.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage:\n" << "    huffman_compressor -c input output\n" << "  huffman_compressor -d input output\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];

    Compressor compressor;

    if (mode == "-c") {
        compressor.compress(input, output);
    }
    else if (mode == "-d") {
        compressor.decompress(input, output);
    }
    else {
        std::cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }

    return 0;
}