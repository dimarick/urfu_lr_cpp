// Угловые скобки означают что файл надо искать в системных директориях (INCLUDE_PATH)
#include <iostream>
// Наши функции для строк
// Двойные кавычки означают что файл надо искать относительно директории текущего файла
#include "strings.h"

using namespace std;

int main() {
    const char *str[] = {"qwerty", "1234567890", "fdsa"};

    // Длина каждой строки в массиве
    for (int i = 0; i < sizeof(str) / sizeof(char *); ++i) {
        cout << "Длина " << str[i] << " = " << stringLength1(str[i]) << endl;
    }

    // Создадим буфер для копирования строк. Его длина намеренно сделана меньше, чем длина самой большой строки
    auto target = new char[8];

    for (int i = 0; i < sizeof(str) / sizeof(char *); ++i) {
        // вторая строка будет 1234567, так как нужно 8 символов вместить еще и \0
        stringCopy(str[i], target, 8);
        cout << "Копия " << str[i] << " = " << target << endl;
    }

    // Не забываем очистить память
    delete[] target;

    // В concatResult будем хранить результат конкатенации всех предыдущих строк
    char *concatResult = new char[1]();
    concatResult[0] = '\0';

    for (int i = 0; i < sizeof(str) / sizeof(char *); ++i) {
        // промежуточная переменная нужна, так как нет автоматического управления памятью
        char *concat = stringConcat(concatResult, str[i]);
        // Не забываем очистить память
        delete[] concatResult;
        concatResult = concat;
        cout << "Сцепление строк: " << str[i] << " = " << concatResult << endl;
    }
}
