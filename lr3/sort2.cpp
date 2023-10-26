#include <iostream>
#include <functional>

#define ITEMS {2, 5,  -8, 1, -4, 6, 3, -5, -9, 13, 0, 4, 9}
#define N1 3
#define N2 8

using namespace std;

void show(const int array[], size_t n) {
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

// сортирока пузырьком
void bubbleSort(int array[], size_t size, const function<int(int a, int b)>& comparator) {
    bool swaps;
    do {
        swaps = false;
        for (size_t i = 0; i < size - 1; i++) {
            //если левый больше правого, то меняем их местами
            if (comparator(array[i], array[i + 1]) > 0) {
                swap(array[i], array[i + 1]);
                swaps = true;
            }
        }
        // если ни одного обмена не произошло за проход - все отсортировано
    } while (swaps);
}

// выбор индекса опорного значения. В данном случае просто первое
size_t getPivot(int *array, size_t size) {
    return 0;
}

void quickSort(int array[], size_t size, size_t pivot, const function<int(int a, int b)>& comparator);

// фасад функции
void quickSort(int array[], size_t size, const function<int(int a, int b)>& comparator) {
    if (size < 2) {
        return;
    }

    auto pivot = getPivot(array, size);
    quickSort(array, size, pivot, comparator);
}

// основная реализация: размещаем все что больше - справа от опорного, все остальное - слева
void quickSort(int array[], size_t size, size_t pivot, const function<int(int a, int b)>& comparator) {
    for (size_t i = 0; i < size; i++) {
        if (i == pivot) {
            continue;
        }

        if (comparator(array[i], array[pivot]) > 0) {
            if (i < pivot) {
                // левое больше
                // кладем справа от опорного
                int t = array[i];
                array[i] = array[pivot - 1];
                array[pivot - 1] = array[pivot];
                array[pivot] = t;
                pivot--;
                // важно вернуться на шаг назад, так как опорник сдвинулся влево
                // чтобы обработать значение рядом с ним еще раз (то что мы поместили в pivot - 1)
                i--;
            }
        } else {
            // правое меньше
            if (i > pivot) {
                // кладем слева от опорного
                int t = array[i];
                array[i] = array[pivot + 1];
                array[pivot + 1] = array[pivot];
                array[pivot] = t;
                pivot++;
                // а тут наоборот, i++ нельзя, иначе мы не обработаем еще раз значение из pivot + 1
            }
        }
    }

    quickSort(array, pivot, comparator);
    quickSort(array + pivot + 1, size - pivot - 1, comparator);
}

int main () {
    int array[] = ITEMS;
    int array2[] = ITEMS;
    size_t n = sizeof(array) / sizeof(*array);

    show(array, n);

    int callStat = 0;

    bubbleSort(array, n,[&callStat](auto a, auto b) {
        callStat++;

        return a - b;
    });

    cout << "Bubble sort comparisons " << callStat << endl;

    callStat = 0;

    quickSort(array2, n,[&callStat](auto a, auto b) {
        callStat++;

        return a - b;
    });

    // Нельзя написать две сортировки и не сравнить их. Самая главная метрика - количество сравнений
    cout << "Quick sort comparisons " << callStat << endl;

    show(array, n);
    show(array2, n);

    quickSort(array2, n,[&callStat](auto a, auto b) {
        if (a % 2 == 0 && b % 2 == 0) {
            // четные по возрастанию
            return a - b;
        } else if (a % 2 != 0 && b % 2 != 0) {
            // нечетные по убыванию
            return b - a;
        }

        // все четные слева, все нечетные справа
        return (b % 2) - (a % 2);
    });

    cout << "Сортировка по возрастанию четных и убыванию нечетных: ";

    show(array2, n);

    // отсортировать фрагмент массива еще проще:
    quickSort(array2 + N1, N2 - N1,[&callStat](auto a, auto b) {
        return a - b;
    });

    cout << "Сортировка от N1 до N2: ";

    show(array2, n);
}
