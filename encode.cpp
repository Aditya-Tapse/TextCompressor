#include <bits/stdc++.h>
#include "huffman.cpp"
using namespace std;

int main(int argc, char* argv[]) {
    
   if (argc != 3) {
        cout << "Invalid input";
		exit(1);
	}
    huffman f(argv[1], argv[2]);
    f.compress();
    cout << "Compressed successfully" << endl;
    
    return 0;
}