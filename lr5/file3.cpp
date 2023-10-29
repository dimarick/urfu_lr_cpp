#include <cstdio>
#include <cerrno>
#include <cstring>

using namespace std;

int main(int arc, char *argv[]) {
    if (arc < 4) {
        printf("Usage: %s key input output", argv[0]);

        return 1;
    }

    auto key = argv[1];
    auto keyLength = strlen(key);
    auto inputName = argv[2];
    auto outputName = argv[3];
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

    int cursor = 0;

    while (!feof(in)) {
        int ic = getc(in);

        if (ic == EOF) {
            break;
        }

        auto c = (char) ic;

        // собственно шифрование
        auto e = c ^ key[cursor % keyLength];

        fputc(e, out);

        cursor++;
    }

    fclose(in);
    fclose(out);

    return 0;
}
