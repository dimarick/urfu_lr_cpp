#include <iostream>
// Хидер с классом нашей строки
#include "Str.h"

using namespace std;

// Отслеживаем работу с памятью: количество аллкаций должно быть равно количеству освбождений памяти
void * operator new(size_t size)
{
    void *p = malloc(size);
    cout << "---- Allocating " << size << "bytes at " << p <<  endl;
    return p;
}

void operator delete(void * p)
{
    cout << "---- Free at " << p <<  endl;
    free(p);
}

int main() {
    // Всевозможные варианты работы с нашим классом
    Str s1("heap string");
    Str s2("static string", true);
    Str s3(s1);
    Str s4(s2);
    Str s5(s3);
    Str *s6 = new Str(s3);
    Str s7;
    s7 = *s6 + s4;

    cout << "s1 " << s1.cStr() << endl;
    cout << "s2 " << s2.cStr() << endl;
    cout << "s3 " << s3.cStr() << endl;
    cout << "s4 " << s4.cStr() << endl;
    cout << "s5 " << s5.cStr() << endl;
    cout << "s6 " << s6->cStr() << endl;
    cout << "s7 " << s7.cStr() << endl;

    Str s8;
    s8 = s7;
    cout << "s7 " << s8.cStr() << endl;
    s8 = s3;
    cout << "s3 " << s8.cStr() << endl;
    s8 = s2;
    cout << "s8 " << s8.cStr() << endl;

    cout << "s1 == s2: " << (s1 == s2) << "(expected 0)" << endl;
    cout << "s1 == s3: " << (s1 == s3) << "(expected 1)" << endl;
    cout << "s1 == s6: " << (s1 == *s6) << "(expected 1)" << endl;
    cout << "s7 != s6: " << (s7 != *s6) << "(expected 1)" << endl;

    Str s9 = s8;
    Str s10;
    s10 = Str("111", true) + s9;
    cout << "s10 " << s10.cStr() << endl;
    Str s11;
    s11 = "aaaa";
    cout << "s11 " << s11.cStr() << endl;
    Str s12 = s11 + s10;
    Str s13 = "bbbb";
    cout << "s12 " << s12.cStr() << endl;
    cout << "s13 " << s13.cStr() << endl;

    delete s6;

    return 0;
}