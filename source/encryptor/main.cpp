#include <fstream>
#include <iostream>
#include "cardanus.h"
#include <string>

using namespace std;

const int   GRID_SIZE = 10;
ifstream    input_file;
ofstream    output_file_grid;
ofstream    output_file_key;
Encryptor   encryptor;
string      text;
string      buf;

int main(int argc, char* argv[]) {
    system("chcp 1251 & cls");
    if (argc == 4) {
        input_file.open(argv[1]);
        output_file_grid.open(argv[2]);
        output_file_key.open(argv[3]);
        if (input_file.is_open()) {
            while (getline(input_file, buf)) {
                text.append(buf);
            }
            Cipher_Set set(encryptor.encrypt(text, 10));
            output_file_grid << set.grid.to_string();
            output_file_key << set.key.to_string();
        } else {
            cout << "Cannot open file" << endl;
        }
    } else {
        cout << "<encrypt> <input_text> <output_grid> <output_key>" << endl;
    }
}