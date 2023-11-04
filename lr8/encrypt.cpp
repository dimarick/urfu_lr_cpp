#include <iostream>
#include <cerrno>
#include <cstring>
#include <fstream>

using namespace std;

// все как в lr5/file3.cpp
int main(int arc, char *argv[]) {
    if (arc < 4) {
        cout << "Usage: " << argv[0] << " key input output" << endl;

        return 1;
    }

    auto key = argv[1];
    auto keyLength = strlen(key);
    auto inputName = argv[2];
    auto outputName = argv[3];

    // но вместо FILE * - ifstream/ofstream
    // можно не указывать флаги, ifstream и так знает что файл открывается для чтения
    ifstream in(inputName);

    if (!in.is_open()) {
        cout << "Файл " << inputName << " не открыт: " << strerror(errno) << endl;

        return 1;
    }

    // а ofstream - для записи
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
        // тут важно явно указать тип char, иначе в out будет выводиться не сам символ, а его код в десятичном формате
        // в отличие от putc, который все равно приведет к char и его выведет
        auto e = (char)(c ^ key[cursor % keyLength]);

        out << e;

        cursor++;
    }

    return 0;
}
