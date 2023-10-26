#include <iostream>
#include <iomanip>
#include <functional>
#include <cfloat>

// Чтобы не писать везде std::cout и т.п.
using namespace std;

// Размерность
#define N 5

// Макросы для расчета среднего
// Полный размер матрицы проще всего
#define MATRIX_SIZE(w, h) ((float)w * (float)h)
// Размер треугольной матрицы - это сумма членов арифметической прогрессии от 1 до N
#define MATRIX_TRIANGLE_SIZE(w, h) ((float)(w + 1) * (float)h / 2)

// Шорткат для объявления лямбды для reduceMatrix. auto очень важны, так как позволяют использовать один макрос для всех T
#define REDUCE_FN(...) [__VA_ARGS__] (auto v, auto r, auto x, auto y) -> auto

// Вспомогательная функция для работы с двумерной матрицей float в стиле функционального программирования
template<typename T> T reduceMatrix(const float *m, size_t w, size_t h, T acc, const std::function<T (float v, T r, size_t x, size_t y)>& fn) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            // Так как информация о типе m[N][N] здесь недоступна, оперируем матрицей как плоским массивом флоатов
            acc = fn(m[i * w + j], acc, j, i);
        }
    }

    return acc;
}

// Вариант со ссылками, используется приведение ссылки к size_t и обратно, т.к. размерность size_t совпадает с размерностью адресов
template<typename T> T* reduceMatrix(const float *m, size_t w, size_t h, T *acc, const std::function<T * (float v, T *r, size_t x, size_t y)>& fn) {
    return (T*)reduceMatrix<size_t>(m, w, h, (size_t)acc, REDUCE_FN(fn) {
        return (size_t) fn(v, (T *) r, x, y);
    });
}

// поиск абсолютного минимума, как начальное значение берется наибольшее возможное значение, чтобы избежать лишних условий
float absMin(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, FLT_MAX, REDUCE_FN() {return min(r, v);});
}

// поиск абсолютного максимума, как начальное значение берется наименьшее возможное значение, чтобы избежать лишних условий
float absMax(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, -FLT_MAX, REDUCE_FN() {return max(r, v);});
}

float lowTriangleMax(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, -FLT_MAX, REDUCE_FN() {
        // элементы выше главной диагонали (x == y) не обрабатываются
        if (x > y) {
            return r;
        }

        return max(r, v);
    });
}

float highTriangleMax(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, -FLT_MAX, REDUCE_FN() {
        // элементы ниже главной диагонали (x == y) не обрабатываются
        if (x < y) {
            return r;
        }

        return max(r, v);
    });
}

float lowTriangleMin(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, FLT_MAX, REDUCE_FN() {
        if (x > y) {
            return r;
        }

        return min(r, v);
    });
}

float highTriangleMin(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, FLT_MAX, REDUCE_FN() {
        if (x < y) {
            return r;
        }

        return min(r, v);
    });
}

float minDiag(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, FLT_MAX, REDUCE_FN() {
        // элементы вне главной диагонали не обрабатываются
        if (x != y) {
            return r;
        }

        return min(r, v);
    });
}


float maxDiag(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, -FLT_MAX, REDUCE_FN() {
        // элементы вне главной диагонали не обрабатываются
        if (x != y) {
            return r;
        }

        return max(r, v);
    });
}

float minDiag2(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, FLT_MAX, REDUCE_FN(w) {
        // Элементы вне второстепенной диагонали не обрабатываются. w - 1 - x инвертирует x так что первый элемент станет последним и наоборот.
        if (y != (w - 1 - x)) {
            return r;
        }

        return min(r, v);
    });
}


float maxDiag2(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, -FLT_MAX, REDUCE_FN(w) {
        // Элементы вне второстепенной диагонали не обрабатываются. w - 1 - x инвертирует x так что первый элемент станет последним и наоборот.
        if (y != (w - 1 - x)) {
            return r;
        }

        return max(r, v);
    });
}

float lowTriangleSum(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, 0.f, REDUCE_FN() {
        if (x > y) {
            return r;
        }

        return r + v;
    });
}

float highTriangleSum(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, 0.f, [] (auto v, float r, auto x, auto y) {
        if (x < y) {
            return r;
        }

        return r + v;
    });
}

// среднее - суть сумма деленная на общий размер матрицы
float absAvg(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, 0.f, [] (auto v, float r, auto x, auto y) {return r + v;}) / MATRIX_SIZE(w, h);
}

// среднее - суть сумма деленная на размер треугольной матрицы
float lowTriangleAvg(const float *m, size_t w, size_t h) {
    return lowTriangleSum(m, w, h) / MATRIX_TRIANGLE_SIZE(w, h);
}

// среднее - суть сумма деленная на размер треугольной матрицы
float highTriangleAvg(const float *m, size_t w, size_t h) {
    return highTriangleSum(m, w, h) / MATRIX_TRIANGLE_SIZE(w, h);
}

// похоже на поиск минимального значения, только сравниваются разности значений с расчитаным предварительно средним
float nearestAvgElement(const float *m, size_t w, size_t h) {
    const float avg = absAvg(m, w, h);

    return reduceMatrix<float>(m, w, h, FLT_MAX, REDUCE_FN(avg) {
        if (abs(r - avg) <= abs(v - avg)) {
            return r;
        }

        return v;
    });
}

// вспомогательная функция создания массива и заполнения значением
float *newFloatArray(size_t n, float value) {
    auto ptr = new float[n]();

    fill(ptr, ptr + n, value);

    return ptr;
}

float *sumRows(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, newFloatArray(h, 0), REDUCE_FN() {
        r[y] = r[y] + v;
        return r;
    });
}


float *sumColumns(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, newFloatArray(w, 0), REDUCE_FN() {
        r[x] = r[x] + v;
        return r;
    });
}

float *minRows(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, newFloatArray(h, FLT_MAX), REDUCE_FN() {
        r[y] = min(r[y], v);
        return r;
    });
}

float *minColumns(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, newFloatArray(w, FLT_MAX), REDUCE_FN() {
        r[x] = min(r[x], v);
        return r;
    });
}

float *maxRows(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, newFloatArray(h, -FLT_MAX), REDUCE_FN() {
        r[y] = max(r[y], v);
        return r;
    });
}

float *maxColumns(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, newFloatArray(w, -FLT_MAX), REDUCE_FN() {
        r[x] = max(r[x], v);
        return r;
    });
}

float *avgRows(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, newFloatArray(h, 0), REDUCE_FN(w, h) {
        r[y] = r[y] + v / MATRIX_SIZE(w, h);
        return r;
    });
}

float *avgColumns(const float *m, size_t w, size_t h) {
    return reduceMatrix<float>(m, w, h, newFloatArray(w, 0), REDUCE_FN(w, h) {
        r[x] = r[x] + v / MATRIX_SIZE(w, h);
        return r;
    });
}

// Для вывода на экран также используем reduceMatrix, только при этом не возвращаем результат
void dump(const float *m, size_t w, size_t h) {
    reduceMatrix<nullptr_t>(m, w, h, nullptr, REDUCE_FN(w) {
        cout << setw(12) << setprecision(5) << v;
        if (x == w - 1) {
            cout << endl;
        }

        return nullptr;
    });
}

int main () {
    float m[N][N];
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            m[i][j] = (float)rand() / 10.f;
        }
    }

    auto mRef = (float *)&m;

    dump(mRef, N, N);

    cout << "минимум матрицы:\t" << absMin(mRef, N, N) << endl;
    cout << "максимум матрицы:\t" << absMax(mRef, N, N) << endl;
    cout << "максимум нижнетреугольной части матрицы:\t" << lowTriangleMax(mRef, N, N) << endl;
    cout << "максимум верхнетреугольной части матрицы:\t" << highTriangleMax(mRef, N, N) << endl;
    cout << "минимум нижнетреугольной части матрицы:\t" << lowTriangleMin(mRef, N, N) << endl;
    cout << "минимум верхнетреугольной части матрицы:\t" << highTriangleMin(mRef, N, N) << endl;
    cout << "минимум главной диагонали матрицы:\t" << minDiag(mRef, N, N) << endl;
    cout << "максимум главной диагонали матрицы:\t" << maxDiag(mRef, N, N) << endl;
    cout << "минимум второстепенной диагонали матрицы:\t" << minDiag2(mRef, N, N) << endl;
    cout << "максимум второстепенной диагонали матрицы:\t" << maxDiag2(mRef, N, N) << endl;
    cout << "среднеарифметическое значение элементов матрицы:\t" << absAvg(mRef, N, N) << endl;
    cout << "среднеарифметическое значение элементов нижнетреугольной части матрицы:\t" << lowTriangleAvg(mRef, N, N) << endl;
    cout << "среднеарифметическое значение элементов верхнетреугольной части матрицы:\t" << highTriangleAvg(mRef, N, N) << endl;

    // временная переменная только для предотвращения утечек памяти
    float *tmp;
    cout << "суммы строк матрицы:\t" << endl;
    dump(tmp = sumRows(mRef, N, N), 1, N);
    delete tmp;
    cout << endl;
    cout << "суммы столбцов матрицы:\t";
    dump(tmp = sumColumns(mRef, N, N), N, 1);
    delete tmp;
    cout << endl;
    cout << "минимальные значения строк матрицы:\t" << endl;
    dump(tmp = minRows(mRef, N, N), 1, N);
    delete tmp;
    cout << endl;
    cout << "минимальные значения столбцов матрицы:\t";
    dump(tmp = minColumns(mRef, N, N), N, 1);
    delete tmp;
    cout << endl;
    cout << "максимальные значения строк матрицы:\t" << endl;
    dump(tmp = maxRows(mRef, N, N), 1, N);
    delete tmp;
    cout << endl;
    cout << "максимальные значения столбцов матрицы:\t";
    dump(tmp = maxColumns(mRef, N, N), N, 1);
    delete tmp;
    cout << endl;
    cout << "среднеарифметические значения строк матрицы:\t" << endl;
    dump(tmp = avgRows(mRef, N, N), 1, N);
    delete tmp;
    cout << endl;
    cout << "среднеарифметические значения столбцов матрицы:\t";
    dump(tmp = avgColumns(mRef, N, N), N, 1);
    delete tmp;
    cout << endl;

    cout << "сумма нижнетреугольной части матрицы:\t" << lowTriangleSum(mRef, N, N) << endl;
    cout << "сумма верхнетреугольной части матрицы:\t" << highTriangleSum(mRef, N, N) << endl;
    cout << "элемент, наиболее близкий по значению к среднеарифметическому:\t" << nearestAvgElement(mRef, N, N) << endl;
}
