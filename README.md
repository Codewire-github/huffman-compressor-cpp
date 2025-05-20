# Huffman File Compressor (C++)

A simple file compression and decompression tool using **Huffman coding**, written in modern **C++**. This project supports compressing arbitrary binary files (e.g., `.txt`, `.pdf`, `.png`, etc.) into a custom `.hc` format and decompressing them back to their original form.

---

## 📦 Features

- Supports any file type (text, image, audio, binary, etc.)
- Custom `.hc` format with embedded Huffman tree
- Efficient compression using Huffman coding
- CLI interface for ease of use
- No external dependencies

---

## 🛠️ Build Instructions

### Requirements

- C++17 or newer
- CMake (version ≥ 3.10)
- A C++ compiler (e.g., `g++`, `clang++`, `MSVC`)

### Build Steps

1. **Clone the repository:**

```bash
git clone https://github.com/Codewire-github/huffman-compressor-cpp.git
cd huffman-compressor-cpp
```
2. **Build with CMake:**
``` bash
mkdir build
cd build
cmake ..
make
```
3. **Run the executable:**
```bash
./huffman-compressor-cpp
```

## 🚀 Usage
### Compress a file:
```bash
./huffman-compressor-cpp -c <input_file> <output_file.hc>
```
📌 The compressed output will be saved as: <output_file.hc>

**Example:**
```bash
./huffman-compressor-cpp -c test.txt
# Output: test.txt.hc
```
### Decompress a file:
```bash
./huffman-compressor-cpp -d <input_compressed_file> <output_file>
```
**Example:**
```bash
./huffman-compressor-cpp -d test.txt.hc test_decoded.txt
```
## 📂 File Structure
```bash
huffman-compressor-cpp/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── src/
│   ├── compressor.cpp
│   └── compressor.hpp
└── build/ (created after building)
```
## 🧠 How It Works

1. Builds a frequency table from the input file
2. Constructs a Huffman tree
3. Encodes the input data into a bitstream
4. Serializes the Huffman tree + bitstream + total bit count
5. Writes to a .hc file
6. Decompressing reverses this process

## 📄 Testing and Results

To validate the correctness and efficiency of the Huffman Compressor, I tested the implementation using both text and binary files.
#### ✅ Text File Test: 300 Paragraphs of Lorem Ipsum

    Input File: lipsum.txt

    Size: ~89 KB

    Content: 300 paragraphs of standard Lorem Ipsum text

    Compressed Output: lipsum.hc

    Size: ~47 KB

    Compression Ratio: ~47%

This significant reduction in size (~47%) demonstrates the effectiveness of Huffman encoding on natural language text. The byte frequency and the generated Huffman codes were printed to verify internal states and validate the compression logic.

## ❗ Limitations
1. Doesn't compress already compressed files (e.g., .zip, .jpg) well
2. Custom format (.hc) — not compatible with other tools 
3. No multi-threading or streaming yet
