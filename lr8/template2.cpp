#include <iostream>

using namespace std;

// Шаблоны getmax для 2 значений и массива
template <class T>
T getmax(T t1, T t2) {
    return t1 > t2 ? t1 : t2;
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
    // проверяем работу
    int i1 = 2, i2 = 5;
    int ints[] = {3, 9, 5, 8};

    cout << "max int " << getmax(i1, i2) << endl;
    cout << "max int " << getmax(ints, sizeof(ints) / sizeof(*ints)) << endl;

}
