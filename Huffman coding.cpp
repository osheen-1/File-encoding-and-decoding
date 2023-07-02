#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;

// Tree node structure
struct Node {
    char symbol;
    int frequency;
    Node* left;
    Node* right;

    Node(char symbol, int frequency, Node* left = nullptr, Node* right = nullptr)
        : symbol(symbol), frequency(frequency), left(left), right(right) {}
};

// Comparator for priority queue based on node frequencies
struct CompareNodes {
    bool operator()(const Node* a, const Node* b) const {
        return a->frequency > b->frequency;
    }
};

// Traverse the Huffman tree and build the lookup table
void buildCodes(Node* root, string code, unordered_map<char, string>& codes) {
    if (root == nullptr)
        return;

    if (root->symbol)
        codes[root->symbol] = code;

    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

// Encode the input data using the lookup table
string huffmanEncoding(string data, unordered_map<char, string>& codes) {
    string encodedData;
    for (char c : data)
        encodedData += codes[c];

    return encodedData;
}

// Decode the encoded data using the Huffman tree
string huffmanDecoding(string encodedData, Node* root) {
    string decodedData;
    Node* currentNode = root;

    for (char bit : encodedData) {
        if (bit == '0')
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;

        if (currentNode->symbol) {
            decodedData += currentNode->symbol;
            currentNode = root;
        }
    }

    return decodedData;
}

// Perform Huffman coding
pair<string, Node*> huffmanCoding(string data) {
    // Calculate symbol frequencies
    unordered_map<char, int> frequency;
    for (char c : data)
        frequency[c]++;

    // Create a priority queue based on frequencies
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;
    for (auto& pair : frequency)
        pq.push(new Node(pair.first, pair.second));

    // Build the Huffman tree
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* merged = new Node('\0', left->frequency + right->frequency, left, right);
        pq.push(merged);
    }

    Node* root = pq.top();

    // Build the lookup table
    unordered_map<char, string> codes;
    buildCodes(root, "", codes);

    // Perform encoding
    string encodedData = huffmanEncoding(data, codes);

    return { encodedData, root };
}

int main() {
    string data = "Huffman coding is a data compression algorithm.";

    // Perform Huffman coding
    pair<string, Node*> encodingResult = huffmanCoding(data);
    string encodedData = encodingResult.first;
    Node* root = encodingResult.second;

    // Perform decoding
    string decodedData = huffmanDecoding(encodedData, root);

    // Output results
    cout << "Original data: " << data << endl;
    cout << "Encoded data: " << encodedData << endl;
    cout << "Decoded data: " << decodedData << endl;

    // Clean up memory
    delete root;

    return 0;
}
