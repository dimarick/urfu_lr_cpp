#include <iostream>
#include "../lr6/Str.h"

using namespace std;

// Перегружаем операторы потоков
ostream& operator << (ostream &os, const Str &str)
{
    // << проще всего
    return os << str.cStr();
}

istream& operator >> (istream &is, Str &str)
{
    // Тут тоже самое, но больше доп. действий.
    // используем std::string, иначе придется повторять довольно длинный фрагмент кода STL
    std::string input;

    is >> input;

    str = input.c_str();

    return is;
}

int main() {
    Str s = "Введите что угодно и нажмите ENTER > ";

    cout << s;
    cin >> s;
    cout << "Вы ввели " << s << endl;

    return 0;
}