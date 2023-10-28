#include "strings.h"

/**
 * Первый способ: просто вызываем strlen
 */
size_t stringLength1(const char *str) {
    return strlen(str);
}

/**
 * Второй способ: просто вызываем strnlen
 */
size_t stringLength2(const char *str) {
    return strnlen(str, 100);
}

/**
 * Третий способ: ищем символ завершения строки
 */
size_t stringLength3(const char *str) {
    for (size_t i = 0; i < 100; ++i) {
        if (str[i] == '\0') {
            return i;
        }
    }

    return 100;
}

/**
 * Копируем строку source в destination размером destinationSize, чтобы не словить переполнение буфера
 * @param source
 * @param destination
 * @param size_t destinationSize
 */
void stringCopy(const char *source, char *destination, size_t destinationSize) {
    for (size_t i = 0; i < destinationSize - 1; ++i) {
        destination[i] = source[i];

        if (source[i] == '\0') {
            return;
        }
    }

    destination[destinationSize - 1] = '\0';
}

/**
 * Если строка str1 меньше, то значение < 0, если больше - значение > 0, для равных строк == 0. При одинаковом начале строк, меньшей считается более короткая строка
 */
int stringCompare(const char *str1, const char *str2) {
    for (size_t i = 0; ; ++i) {
        int diff = str1[i] - str2[i];
        if (diff != 0) {
            return diff;
        }

        if (str1[i] == '\0') {
            return 0;
        }
    }
}

/**
 * Добавляет str2 в конец str1 в новом буфере, который после использования должен быть удален с помощью delete
 */
char *stringConcat(const char *str1, const char *str2) {
    size_t n1 = strlen(str1);
    size_t n2 = strlen(str2);
    auto buffer = new char[n1 + n2]();

    memcpy(buffer, str1, n1);
    memcpy(buffer + n1, str2, n2);

    return buffer;
}
