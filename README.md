# Huffman File Compressor

A lossless compression tool implementing Huffman coding in C. This program compresses files by analyzing byte frequencies, generating optimal prefix codes, and producing a compressed binary output.

## Features

- ✅ Counts byte frequencies in input files
- ✅ Builds Huffman trees and generates optimal codes
- ✅ Compresses files using variable-length bit encoding
- ✅ Handles bit-level I/O for efficient storage
- ✅ Cross-platform (Linux/macOS/Windows)

## Installation

### Build from Source
```bash
git clone https://github.com/yourusername/huffman-compressor.git
cd huffman-compressor
make
```

## Usage

### Compress a file
```bash
./huffman -c input.txt output.huff
```

### Decompress a file
```bash
./huffman -d output.huff decompressed.txt
```

### Options
```
-c  Compress mode
-d  Decompress mode
-v  Verbose output (show compression stats)
```

## File Format
```
[HEADER]
- Frequency table (256 uint32 values)

[DATA]
- Bit-packed Huffman codes
- EOF marker
```

## Technical Details
- **Bit Writing**: Uses MSB-first bit packing
- **Tree Storage**: Serializes frequency table for reconstruction
- **Padding**: Automatically pads final byte with zeros

## Example
```c
// Compression workflow
1. Read file → count frequencies → build tree
2. Generate codes → write header → encode data
3. Write final bits with padding
```

## License
MIT License
```

To use this:
1. Copy the entire content above
2. Paste into a new file named `README.md` in your project root
3. Customize:
   - Replace `yourusername` with your GitHub handle
   - Add your specific build instructions if not using `make`
   - Update license if not using MIT


