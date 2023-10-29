#include <iostream>
#include <cstring>
#include <regex>

int getPageNumber(char *buffer);
const char *getPageContent(char *buffer);


using namespace std;

// Используем Raw string literals
static const regex pageStartRegex(R"(\s*-\s*(\d+)\s*-\s*)");

int main(int arc, char *argv[]) {
    if (arc < 3) {
        cout << "Usage: " << argv[0] << " input output" << endl;

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

        // Обработка разрыва страниц
        if (c == '\f') {
            int pageNo = getPageNumber(pageBuffer);
            auto pageContent = getPageContent(pageBuffer);

            fprintf(out, "%s\n%d\n\f", pageContent, pageNo);

            cursor = 0;
        }

        pageBuffer[cursor++] = c;
        pageBuffer[cursor] = '\0';
    }

    fclose(in);
    fclose(out);
    free(pageBuffer);

    return 0;
}

int getPageNumber(char *buffer) {
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

const char *getPageContent(char *buffer) {
    smatch m;
    auto s = string(buffer);

    if (regex_search(s, m, pageStartRegex)) {
        if (m.size() > 1) {
            return buffer + m.position(0) + m.length();
        }
    }

    return nullptr;
}
