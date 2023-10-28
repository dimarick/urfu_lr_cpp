#include <iostream>
#include <cstring>
// Наши функции для строк
#include "strings.h"

using namespace std;

int main() {
    // Размеры строк в символах
    size_t n1 = sizeof("qwerty") / sizeof(char);
    size_t n2 = sizeof("1234567890") / sizeof(char);
    // calloc принмает количество элементов и их размер, в отличие от malloc, принимающей размер буфера в байтах. Также calloc гарантирует что буфер будет заполнен нулями.
    // Причем, в общем случаае, вызов calloc дешевле, чем вызов malloc + обнуление полученного буфера, но дороже чем простой вызов malloc
    // Наша программа одинаково работает и с обнуленным и не обнуленным буфером, поэтому с т.з. времени выполнения, malloc предпочтительнее
    char *str1 = (char *)calloc(n1, sizeof(char));
    char *str2 = (char *)calloc(n2, sizeof(char));
    char *str3 = (char *)calloc(n2, sizeof(char));

    stringCopy("qwerty", str1, n1);
    stringCopy("1234567890", str2, n2);
    stringCopy("1234567890", str3, n2);

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