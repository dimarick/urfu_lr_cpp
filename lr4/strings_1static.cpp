#include <iostream>
#include <cstring>
// Наши функции для строк
#include "strings.h"

using namespace std;

int main() {
    // Объявляем строки
    const char str1[] = "qwerty";
    // Эта строка изменяется функцией копирования
    char str2[] = "1234567890";
    const char str3[] = "1234567890";

    cout << "1. Длина str1: " << stringLength1(str1) << ". Длина str2: " << stringLength1(str2) << endl;
    cout << "2. Длина str1: " << stringLength2(str1) << ". Длина str2: " << stringLength2(str2) << endl;
    cout << "3. Длина str1: " << stringLength3(str1) << ". Длина str2: " << stringLength3(str2) << endl;

    stringCopy(str1, str2, strlen(str2) + 1);
    cout << "Результат копирования " << str2 << endl;

    auto equality = stringCompare(str1, str2);
    cout << "Результат сравнения " << equality << endl;

    auto equality2 = stringCompare(str1, str3);
    cout << "Результат сравнения " << equality2 << endl;

    auto concat = stringConcat(str1, str3);
    cout << "Результат сцепления " << concat << endl;
    // Не забываем очистить память
    delete concat;
}
