#include <iostream>
#include <cstring>

using namespace std;

template <class T>
T getmax(T t1, T t2) {
    return t1 > t2 ? t1 : t2;
}
char *getmax(char *t1, char *t2) {
    return strcmp(t1, t2) > 0 ? t1 : t2;
}

template <class T>
T getmax(T t[], size_t size) {
    T result = t[0];

    for (int i = 1; i < size; ++i) {
        if (t[i] > result) {
            result = t[i];
        }
    }

    return result;
}

int main() {
    int i1 = 2, i2 = 5;
    int ints[] = {3, 9, 5, 8};
    const char *str1 = "строка 1";
    const char *str2 = "строка 2";

    cout << "max int " << getmax(i1, i2) << endl;
    cout << "max str " << getmax(str1, str2) << endl;
    cout << "max int " << getmax(ints, sizeof(ints) / sizeof(*ints)) << endl;
}
