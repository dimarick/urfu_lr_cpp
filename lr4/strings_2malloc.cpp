#include <iostream>
#include <cstring>
// Наши функции для строк
#include "strings.h"

using namespace std;

int main() {
    // Размеры строк в байтах
    size_t n1 = sizeof("qwerty");
    size_t n2 = sizeof("1234567890");
    // Выделим память для строк. n1 уже включает в себя \0, так как получен через sizeof
    // malloc оперирует байтами,
    char *str1 = (char *)malloc(n1);
    char *str2 = (char *)malloc(n2);
    char *str3 = (char *)malloc(n2);

    // а stringCopy - символами
    // И хотя на привычных платформах sizeof(char) == 1, не стоит полагаться что это абсолютно везде так.
    stringCopy("qwerty", str1, n1 / sizeof(char));
    stringCopy("1234567890", str2, n2 / sizeof(char));
    stringCopy("1234567890", str3, n2 / sizeof(char));

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

    // как выделяли, так и освобождаем
    delete concat;
    free(str1);
    free(str2);
    free(str3);
}