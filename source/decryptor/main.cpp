#include "interface.h"
#include <iostream>

using namespace std;

int main(void) {
    string      command_line;
    Interface   interface;

    system("chcp 1251 & cls");
    interface.exec("help");
    cout << "--> ";
    while (1) {
        try {
            while (getline(cin, command_line)) {
                if (!command_line.empty()) {
                    cout << endl << interface.exec(command_line);
                }
                cout << endl <<  "--> ";
            } 
        } catch (...) {
            cout << "Undefined command" << endl <<  "--> ";
        }
    }

    return 0;
}