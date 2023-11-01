#include <iostream>
#include <cerrno>
#include <cstring>
#include <fstream>

using namespace std;

int main(int arc, char *argv[]) {
    if (arc < 4) {
        cout << "Usage: " << argv[0] << " key input output" << endl;

        return 1;
    }

    auto key = argv[1];
    auto keyLength = strlen(key);
    auto inputName = argv[2];
    auto outputName = argv[3];
    ifstream in(inputName);

    if (!in.is_open()) {
        cout << "Файл " << inputName << " не открыт: " << strerror(errno) << endl;

        return 1;
    }

    ofstream out(outputName);

    if (!out.is_open()) {
        cout << "Файл " << outputName << " не открыт: " << strerror(errno) << endl;

        return 1;
    }

    int cursor = 0;

    while (!in.eof()) {
        char c;

        // in >> c работает неправильно: теряет все переносы строк
        in.get(c);

        // собственно шифрование
        auto e = (char)(c ^ key[cursor % keyLength]);

        out << e;

        cursor++;
    }

    return 0;
}
