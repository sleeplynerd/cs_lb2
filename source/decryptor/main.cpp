#include "interface.h"
#include <iostream>

using namespace std;

int main(void) {
    string      command_line;
    Interface   interface;

    system("chcp 1251 & cls");
    interface.exec("help");
    cout << "-->";
    while (getline(cin, command_line)) {
        cout << endl << interface.exec(command_line);
        cout << endl <<  "-->";
    }

    return 0;
}