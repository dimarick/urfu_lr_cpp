#include <iostream>

using namespace std;

template <class T>
void Sort(T array[], size_t size) {
    for (size_t i = 0; i < size - 1; ++i) {
        for (size_t j = size - 1; i < j; --j) {
            if (array[i] > array[j]) {
                T tmp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = tmp;
            }
        }
    }
}

class Money {
    long dollars = 0;
    int cents = 0;
public:
    Money() = default;
    Money(long d, int c) {
        dollars = d;
        cents = c;
    }

    int operator > (const Money&) const;
    friend ostream& operator << (ostream& os, Money& money);
};

int Money::operator > (const Money& money) const {
    return dollars > money.dollars || (dollars == money.dollars && cents > money.cents);
}

ostream& operator << (ostream& os, Money& money) {
    return os << "$" << money.dollars << "." << money.cents;
}

int main() {
    Money moneys[] = {
        Money(19, 10),
        Money(99, 99),
        Money(99, 95),
        Money(19, 95),
    };

    Sort(moneys, sizeof(moneys) / sizeof(*moneys));

    for (int i = 0; i < sizeof(moneys) / sizeof(*moneys); ++i) {
        cout << "moneys[" << i << "] = " << moneys[i] << endl;
    }
}
