#ifndef LR_CPP_STRINGS_H
#define LR_CPP_STRINGS_H

#include <cstring>

/**
 * Первый способ: просто вызываем strlen
 */
// extern указывает что реализация размещается в другом объектном файле.
extern size_t stringLength1(const char *str);
/**
 * Второй способ: просто вызываем strnlen
 */
extern size_t stringLength2(const char *str);
/**
 * Третий способ: ищем символ завершения строки
 */
extern size_t stringLength3(const char *str);
/**
 * Копируем строку source в destination размером destinationSize, чтобы не словить переполнение буфера
 * @param source
 * @param destination
 * @param size_t destinationSize
 */
extern void stringCopy(const char *source, char *destination, size_t destinationSize);
/**
 * Если строка str1 меньше, то значение < 0, если больше - значение > 0, для равных строк == 0. При одинаковом начале строк, меньшей считается более короткая строка
 */
extern int stringCompare(const char *str1, const char *str2);
/**
 * Добавляет str2 в конец str1 в новом буфере, который после использования должен быть удален с помощью delete
 */
extern char *stringConcat(const char *str1, const char *str2);

#endif
