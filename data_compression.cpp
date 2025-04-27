#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

// Node for priority queue (we use pair instead of class/struct)
typedef pair<int, string> PQElement;

// Frequency map
unordered_map<char, int> buildFrequency(const string& text) {
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }
    return freq;
}

// Build Huffman codes from frequency
void buildHuffmanCodes(const unordered_map<char, int>& freq, unordered_map<char, string>& huffmanCode) {
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

    for (auto& pair : freq) {
        // We encode single characters as strings
        pq.push({pair.second, string(1, pair.first)});
    }

    // Build Huffman Tree (logic-wise, not actual tree)
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        for (char ch : left.second) {
            huffmanCode[ch] = "0" + huffmanCode[ch];
        }
        for (char ch : right.second) {
            huffmanCode[ch] = "1" + huffmanCode[ch];
        }

        pq.push({left.first + right.first, left.second + right.second});
    }
}

// Compress text using Huffman codes
string compressText(const string& text, const unordered_map<char, string>& huffmanCode) {
    string compressed = "";
    for (char ch : text) {
        compressed += huffmanCode.at(ch);
    }
    return compressed;
}

// Decompress binary string using Huffman codes
string decompressText(const string& compressed, const unordered_map<string, char>& reverseCode) {
    string decompressed = "";
    string current = "";

    for (char bit : compressed) {
        current += bit;
        if (reverseCode.count(current)) {
            decompressed += reverseCode.at(current);
            current = "";
        }
    }
    return decompressed;
}

// Main
int main() {
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cout << "Failed to open input.txt\n";
        return 1;
    }

    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    cout << "Original text:\n" << text << "\n";

    unordered_map<char, int> freq = buildFrequency(text);
    unordered_map<char, string> huffmanCode;

    buildHuffmanCodes(freq, huffmanCode);

    string compressed = compressText(text, huffmanCode);
    cout << "\nCompressed binary string:\n" << compressed << "\n";

    // Build reverse map for decompression
    unordered_map<string, char> reverseCode;
    for (auto& pair : huffmanCode) {
        reverseCode[pair.second] = pair.first;
    }

    string decompressed = decompressText(compressed, reverseCode);
    cout << "\nDecompressed text:\n" << decompressed << "\n";

    // Optionally save compressed data
    ofstream compressedFile("compressed.txt");
    compressedFile << compressed;
    compressedFile.close();

    return 0;
}