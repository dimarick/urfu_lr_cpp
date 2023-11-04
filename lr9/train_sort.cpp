#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

template <class T>
class TStack {
protected:
    size_t maxItems = 0;
    size_t numItems = 0;
    T *item;
public:
    explicit TStack(size_t maxItems = 10) {
        this->maxItems = maxItems;
        item = new T[maxItems];
    };

    ~TStack() {
        delete[] item;
    }

    size_t getMaxItems() {
        return maxItems;
    }

    size_t getNumItems() {
        return numItems;
    }

    bool empty() {
        return numItems == 0;
    }
    void push(T value);
    T pop();
};

unsigned long long threeStackSort(TStack<int> &in, TStack<int> &out, TStack<int> &deadEnd);

template<class T>
void TStack<T>::push(T value) {
    if (numItems >= maxItems) {
        throw overflow_error("cannot push to fulfilled stack");
    }
    item[numItems++] = value;
}

template<class T>
T TStack<T>::pop() {
    if (numItems == 0) {
        throw overflow_error("cannot pop from empty stack");
    }
    return item[--numItems];
}

int main (int argc, char **argv) {
    int n = 10;
    if (argc > 1) {
        n = atoi(argv[1]);
    }
    int t = n / 2;
    if (argc > 2) {
        t = atoi(argv[2]);
    }

    int order[n];

    for (int i = 0; i < n; ++i) {
        order[i] = i;
    }

    shuffle(order, order + n, mt19937(random_device()()));

    TStack<int> unsortedTrain(n);
    TStack<int> sortedTrain(n);
    TStack<int> deadEnd(t);

    for (int i = 0; i < n; ++i) {
        unsortedTrain.push(order[i]);
    }

    if (n < 1000) {
        cout << "Unsorted train: ";

        for (int i = n - 1; i >= 0; --i) {
            cout << "[" << order[i] << "]-";
        }

        cout << "/˳˳_˳˳\\" << endl;
    }

    auto count = threeStackSort(unsortedTrain, sortedTrain, deadEnd);

    cout << "Train was sorted after " << (unsigned long long)count << " ops" << endl;

    if (n < 1000) {
        cout << "Unsorted train: ";

        while (!unsortedTrain.empty()) {
            cout << "[" << unsortedTrain.pop() << "]-";
        }

        cout << "/˳˳_˳˳\\" << endl;

        cout << "Dead end: ";

        while (!deadEnd.empty()) {
            cout << "[" << deadEnd.pop() << "]-";
        }

        cout << "/˳˳_˳˳\\" << endl;

        cout << "Sorted train: ";

        while (!sortedTrain.empty()) {
            cout << "[" << sortedTrain.pop() << "]-";
        }

        cout << "/˳˳_˳˳\\" << endl;
    }
}

/**
 * Есть несколько отличий от классической стековой сортировки:
 * 1. Мы используем 3 стека: входной, выходной и тупик
 * 2. Мы предполагаем что вагоны пронумерованы от 0 до (n-1).
 * 3. Классическая стековая сортировка эквивалентна нашей, при размере стека равном 1
 * 4. Классические алгоритмы сортировки оптимизируются для минимизации количества сравнений и обменов, в нашей задаче цель оптимизации - минимизация перемещений вагонов
 *
 * Произвольная нумерация сводится к этой же задаче за N * log (N), так как мы можем каждому значению сопоставить порядковый номер, отсортировав Quicksort
 * Идея в том, что мы запоминаем, в каком стеке следующий вагон, после нахождения текущего
 *
 * Реализация предполагает что вагоны пронумерованы от 0 до (n-1)
 * Асимптотическая сложность решения в любом случае O(N^2)
 * Использование n бит (char locations[n] -
 * в реализации байт для удобства и потенциальной поддержки N тупиков) памяти ускоряет алгоритм в среднем в 2 раза,
 * увеличивает количество лучших случаев c 1 до 2*n (кроме 12345, лучшим случаем станут также 12345, 21345, 32145, 42135 и т.п.)
 * худший случай примет вид 13598642, ожидается что на практике он будет встречаться реже чем 987654321
 * Сортировка с большим количеством тупиков будет ускорять алгоритм в N раз, где N = количество тупиков + 1
 */

/**
 * @param in
 * @param out
 * @param deadEnd
 */
unsigned long long threeStackSortDead(int start, TStack<int> &in, TStack<int> &out, TStack<int> &deadEnd) {
    size_t n = in.getNumItems();

    // "Уравниваем в правах" тупик и несортированный состав:
    TStack<int> *src[2] = {&in, &out};
    // Запоминаем в какой ветке какие вагоны размещены
    char locations[n];
    // считаем операции для статистики
    auto count = 0ull;

    // начальное состояние - все вагоны в in
    for (int i = 0; i < n; ++i) {
        locations[i] = 0;
    }

    if (start < 0) {
        start = 0;
    }

    for (int i = start; i < n + start; ++i) {
        // определяем где лежит текущий вагон
        auto location = locations[i - start];
        auto currentSource = src[location];
        // а соседний стек назначим тупиком
        auto deadLocation = (char)((location + 1) % 2);
        auto currentDead = src[deadLocation];
        bool found = false;

        while (!currentSource->empty()) {
            auto carriage = currentSource->pop();

            if (carriage == i) {
                deadEnd.push(carriage);
                found = true;

                // Больше некуда сортировать, выходим
                if (deadEnd.getNumItems() == deadEnd.getMaxItems()) {
                    return count;
                }
                break;
            }

            // ставим в тупик
            currentDead->push(carriage);

            // запоминаем в каком стеке искать следующее значение
            locations[carriage - start] = deadLocation;
            // статистика
            count++;
        }

        if (!found) {
            throw logic_error("next carriage not found");
        }
    }

    return count;
}

/**
 * @param in
 * @param out
 * @param deadEnd
 * @return
 */
unsigned long long threeStackSort(TStack<int> &in, TStack<int> &out, TStack<int> &deadEnd) {
    if (in.getNumItems() > out.getMaxItems()) {
        throw logic_error("out stack size is too small");
    }

    auto count = 0ull;

    int start = (int)(in.getNumItems() - deadEnd.getMaxItems());
    size_t sortedCount = 0;

    while (!in.empty()) {
        // Размер тупика слишком мал:
        // тогда сортируем в тупик пока он не заполнится
        count += threeStackSortDead(start, in, out, deadEnd);
        start -= (int)deadEnd.getNumItems();

        // то что осталось на выходе копируем обратно
        count += out.getNumItems();
        while (out.getNumItems() > sortedCount) {
            in.push(out.pop());
        }

        // из тупика копируем результат на выход
        count += deadEnd.getNumItems();
        while (!deadEnd.empty()) {
            out.push(deadEnd.pop());
        }

        sortedCount = out.getNumItems();
    }

    return count;
}
