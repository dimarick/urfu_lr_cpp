// stdio.h, string.h и другие устарели, вместо них следует использовать cstdio cstring etc
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cctype>

size_t searchLine(char *search, size_t start, char *buffer);

using namespace std;

int main() {
    char name[50];
    char search[50];
    FILE *in, *out;
    printf("Введите имя файла для просмотра: ");
    // Очень важно ограничить число символов, иначе это будет самая классическая уязвимость переполнения буфера
    scanf("%49s", name);
    printf("Введите строку для поиска: ");
    scanf("%49s", search);

    if ((in = fopen(name, "r")) == nullptr) {
        // Важная черта хорошей программы - корректная обработка ошибок и информативность сообщений о них
        printf("Файл %s не открыт: %s", name, strerror(errno));

        return 1;
    }

    if ((out = fopen("matches.txt", "w")) == nullptr) {
        printf("Файл %s не открыт: %s", name, strerror(errno));

        // Закрываем ранее открытый ресурс
        fclose(in);

        return 1;
    }

    // Статистика по символам
    int graphicalChars = 0;
    int nonGraphicalChars = 0;

    // Храним номера строк для вывода информации с результатами поиска в файл
    int line = 1;
    // и номер символа в строке
    size_t column = 0;
    // Буфер под хранение текущей строки
    size_t lineBufferSize = 128;
    auto lineBuffer = (char *) realloc(nullptr, lineBufferSize * sizeof(char));

    while (!feof(in)) {
        int sch = getc(in);
        if (sch == EOF) {
            printf("EOF\n");
            continue;
        }

        // Буфер закончился. -1 чтобы в конце оставить место под \0
        if (column >= lineBufferSize - 1) {
            // Удваиваем размер буфера
            lineBufferSize *= 2;

            // realloc расширяет или заново выделяет память
            auto newBuffer = (char *) realloc(lineBuffer, lineBufferSize * sizeof(char));

            if (newBuffer == nullptr) {
                free(lineBuffer);
                fclose(in);
                fclose(out);

                printf("Не удалось выделить %zu байт памяти", lineBufferSize);

                return 1;
            }

            lineBuffer = newBuffer;
        }

        auto c = (char) sch;

        // собираем статистику
        if (!iscntrl(c) && c != ' ') {
            graphicalChars++;
        } else {
            nonGraphicalChars++;
        }

        // заполняем буфер
        lineBuffer[column++] = c;
        lineBuffer[column] = '\0';


        // в конце строки
        if (c == '\n') {
            size_t searchStart = 0;
            size_t foundAt = 0;

            // находим и выводим все соотвествия подстроки
            while ((foundAt = searchLine(search, searchStart, lineBuffer)) != -1) {
                // сохраняем результат в файл
                fprintf(out, "%s:%d:%zu:%s", name, line, foundAt + 1, lineBuffer);

                // выводим на экран буфер до соответствия
                for (size_t i = searchStart; i < foundAt; ++i) {
                    putchar(lineBuffer[i]);
                }

                searchStart = foundAt + strlen(search);

                // включаем красную подсветку
                printf("\u001b[1;31m");

                // выводим соотвествие
                for (size_t i = foundAt; i < searchStart; ++i) {
                    putchar(lineBuffer[i]);
                }
                // выключаем красный
                printf("\u001b[0;0m");
            }

            // выводим остаток строки после всех соответствий
            for (size_t i = searchStart; lineBuffer[i] != '\0'; ++i) {
                putchar(lineBuffer[i]);
            }

            // обновляем позицию в файле
            line++;
            column = 0;
        }
    }

    // освобождаем ресурсы
    free(lineBuffer);
    fclose(in);
    fclose(out);

    // выводим статистику
    printf("Графических символов %d, неграфических %d\n", graphicalChars, nonGraphicalChars);
}

size_t searchLine(char *search, size_t start, char *buffer) {
    auto result = strstr(buffer + start, search);

    if (result == nullptr) {
        return -1;
    }

    return (size_t)result - (size_t)buffer;
}
