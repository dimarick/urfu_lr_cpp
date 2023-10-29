#include <cstdio>
#include <cerrno>
#include <cstring>
#include <regex>

int getPageNumber(const char *buffer);
const char *getPageContent(const char *buffer);

using namespace std;

// Используем Raw string literals. Объявляем статически, чтобы компилировать 1 раз, а использовать многократно в разных функциях
static const regex pageStartRegex(R"(\s*-\s*(\d+)\s*-\s*)");

int main(int arc, char *argv[]) {
    if (arc < 3) {
        printf("Usage: %s input output", argv[0]);

        return 1;
    }

    auto inputName = argv[1];
    auto outputName = argv[2];
    FILE *in;

    if ((in = fopen(inputName, "r")) == nullptr) {
        printf("Файл %s не открыт: %s", inputName, strerror(errno));

        return 1;
    }

    FILE *out;

    if ((out = fopen(outputName, "w")) == nullptr) {
        fclose(in);

        printf("Файл %s не открыт: %s", outputName, strerror(errno));

        return 1;
    }

    size_t pageBufferSize = 64;
    char *pageBuffer = (char *) malloc(pageBufferSize * sizeof(char));
    size_t cursor = 0;

    while (!feof(in)) {
        int ic = getc(in);

        if (ic == EOF) {
            break;
        }

        auto c = (char)ic;

        // Обработка разрыва страниц
        if (c == '\f') {
            // Парсим номер страницы из начала буфера
            int pageNo = getPageNumber(pageBuffer);

            // Если не получилось - сохраняем как было
            if (pageNo == -1) {
                fprintf(out, "%s\f", pageBuffer);
                continue;
            }

            // Находим начало содержимого после номера страницы
            auto pageContent = getPageContent(pageBuffer);

            // Форматируем согласно ТЗ
            fprintf(out, "%s\n%d\n\f", pageContent, pageNo);

            cursor = 0;
        }

        if (cursor >= pageBufferSize - 1) {
            // Удваиваем размер буфера
            pageBufferSize *= 2;

            // realloc расширяет или заново выделяет память
            auto newBuffer = (char *) realloc(pageBuffer, pageBufferSize * sizeof(char));

            if (newBuffer == nullptr) {
                free(pageBuffer);
                fclose(in);
                fclose(out);

                printf("Не удалось выделить %zu байт памяти", pageBufferSize);

                return 1;
            }

            pageBuffer = newBuffer;
        }

        pageBuffer[cursor++] = c;
        pageBuffer[cursor] = '\0';
    }

    fclose(in);
    fclose(out);
    free(pageBuffer);

    return 0;
}

/**
 * Парсит номер страницы в формате "- XXX -" в начале страницы текста
 */
int getPageNumber(const char *buffer) {
    // Используем Raw string literals
    smatch m;
    auto s = string(buffer);

    if (regex_search(s, m, pageStartRegex)) {
        if (m.size() > 1) {
            int result = atoi(m[1].first.base());

            return result;
        }
    }

    return -1;
}

/**
 * Находит начало текста, следующего за номером страницы
 */
const char *getPageContent(const char *buffer) {
    smatch m;
    auto s = string(buffer);

    // Используем ту же регулярку
    if (regex_search(s, m, pageStartRegex)) {
        if (m.size() > 1) {
            // Но теперь берем смещение конца матча
            return buffer + m.position(0) + m.length();
        }
    }

    return nullptr;
}
