#include "compressor.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage:\n" << "    huffman_compressor_cpp -c <input_file> <output_file>\n" << "  huffman_compressor_cpp -d <input_file> <output_file>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];

    Compressor compressor;

    if (mode == "-c") {
        compressor.compress(input, output);
        std::cout << "File Compressed Successfully" << std::endl;
    }
    else if (mode == "-d") {
        compressor.decompress(input, output);
        std::cout << "File Decompressed Successfully" << std::endl;
    }
    else {
        std::cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }

    return 0;
}