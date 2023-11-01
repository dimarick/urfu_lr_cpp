#include <iostream>
#include "../lr6/Str.h"

using namespace std;

ostream& operator << (ostream &os, const Str &str)
{
    return os << str.cStr();
}

istream& operator >> (istream &is, Str &str)
{
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