#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <vector>

using namespace std;

// Structure for a Huffman tree 
struct HuffmanNode {
    char data;
    unsigned frequency;
    HuffmanNode *left, *right;

    HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparison function for the priority queue
struct CompareNodes {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

// Function to build the Huffman tree
HuffmanNode* buildHuffmanTree(const map<char, unsigned>& frequencies) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (const auto& pair : frequencies) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Build the Huffman tree by merging nodes
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* newNode = new HuffmanNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    return pq.top();
}

// Function to generate Huffman codes
void generateHuffmanCodes(HuffmanNode* root, string code, map<char, string>& huffmanCodes) {
    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->data] = code;
        return;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Function to encode a file using Huffman coding
void encodeFile(const string& inputFile, const string& outputFile) {
    // Count the frequency of each character in the file
    map<char, unsigned> frequencies;
    ifstream file(inputFile);
    char ch;
    while (file.get(ch)) {
        frequencies[ch]++;
    }
    file.close();

    // Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(frequencies);

    // Generate Huffman codes
    map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // Encode the file and write to the output file
    ofstream outFile(outputFile);
    file.open(inputFile);
    string buffer; // to store the encoded bits
    while (file.get(ch)) {
        buffer += huffmanCodes[ch];
    }

    // Write the encoded data to the output file
    while (!buffer.empty()) {
    // Take up to 8 bits at a time and write to the output file
    string byte = buffer.substr(0, min(8, static_cast<int>(buffer.size())));
    buffer = buffer.substr(min(8, static_cast<int>(buffer.size())));

    // Pad the last byte if necessary
    byte += string(8 - byte.size(), '0');

    unsigned char byteValue = stoi(byte, nullptr, 2); // Convert binary string to byte
    outFile << byteValue;
}

    

    // Close files and release memory
    file.close();
    outFile.close();
    // You need to implement a function to delete the Huffman tree nodes
}
// Function to delete a Huffman tree
void deleteTree(HuffmanNode* root) {
    if (root == nullptr)
        return;
    
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}




int main() {
    string inputFile = "C:\\Users\\ashua\\Desktop\\age.txt";
    string compressedFile = "C:\\Users\\ashua\\Desktop\\agecomp.bin";

    encodeFile(inputFile, compressedFile);

    cout << "File compressed successfully." << endl;

    return 0;
}
