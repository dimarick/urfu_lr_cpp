#include <iostream>
#include <sstream>

// чтобы не писать везде std::cout и т.п.
using namespace std;

// Принудительно используем 64-битные значения, чтобы ограничения были одинаковые для любых платформ
long long power(long long base, unsigned short power);

int main (int argc, char *argv[]) {
    // Показываем справку
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <base> <power>" << endl;
        return 0;
    }

    // Разбираем ввод
    auto base = stoll(argv[1]);
    auto powerArg = stol(argv[2]);

    // вычисляем и выводим ответ
    cout << power(base, powerArg) << endl;
}

/**
 * @param base основание
 * @param power степень
 * @return результат
 */
long long power(long long base, unsigned short power) {
    auto result = 1LL;

    for (int i = 0; i < power; i++) {
        auto x = result * base;
        // проверка целочисленного переполнения
        if (x / base != result) {
            // Удобно и безопасно собираем строку с ошибкой
            stringstream err;
            err << "Integer overflow when power " << base << " to " << power << endl;
            throw overflow_error(err.str());
        }

        // если все ок, продолжаем работу
        result = x;
    }

    return result;
}
