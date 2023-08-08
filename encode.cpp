#include <iostream>
#include "huffman.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    
    huffman f(argv[1], argv[2]);
    f.compress();
    cout << "Compressed successfully" << endl;
    
    return 0;
}