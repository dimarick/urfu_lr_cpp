#include <iostream>
#include <utility>
#include <cstring>
#include <functional>

using namespace std;

// объявляем типы данных
template <typename T> class SingleLinkedListNodeReference;
template <typename T> class SingleLinkedListNode;
template <typename T> class SingleLinkedList;
struct Bus;

// и функции
void showStat(const SingleLinkedList<Bus> &list);
const Bus *moveBus(long id, SingleLinkedList<Bus> &from, SingleLinkedList<Bus> &to);
void showUsage();
int random(int min, int max);

// Класс - узел списка: содержит тело объекта и ссылку на следующий объект
template <typename T>
class SingleLinkedListNode {
private:
    T _value;
    SingleLinkedListNode<T> *_next;
public:
    SingleLinkedListNode() = default;
    explicit SingleLinkedListNode(T *value) : _value(*value) {}
    ~SingleLinkedListNode() {
        delete _next;
    }

    const T *get() const {
        return &_value;
    }

    SingleLinkedListNode<T> *next() const {
        return _next;
    }

    void next(SingleLinkedListNode<T> *next) {
        _next = next;
    }

    /**
     * Функция поиска: возвращает найденный первый найденный (searchFn вернула true) элемент. Чтобы найти все подходящие элементы, вызывайте ее снова с ref.node.next()
     * Важный момент: SingleLinkedListNodeReference возвращаем по значению: всего лишь 2 указателя скопировать, но избавляет нас от ручного управления памятью
     */
    SingleLinkedListNodeReference<T> search(const function<bool(const T &value)> &searchFn) const {
        auto node = (SingleLinkedListNode<T> *) this;
        auto prev = (SingleLinkedListNode<T> *) nullptr;

        while (node != nullptr) {
            if (searchFn(*node->get())) {
                return SingleLinkedListNodeReference<T>(node, prev);
            }

            prev = node;
            node = node->next();
        }

        return SingleLinkedListNodeReference<T>(nullptr, nullptr);
    }
};

// Ссылка на ноду, содержит 2 указателя: на текущую ноду, и на предыдущую: это потребуется для удаления
template <typename T>
class SingleLinkedListNodeReference {
private:
    SingleLinkedListNode<T> *_node;
    // Если prev == nullptr, то нода - первая в списке
    // Несмотря на то что prev->next == node в обычном случае,
    // избыточность внесена для контроля ошибок (см. SingleLinkedList::detach) и обработки головы списка
    SingleLinkedListNode<T> *_prev;
public:
    SingleLinkedListNodeReference(SingleLinkedListNode<T> *node, SingleLinkedListNode<T> *prev) : _node(node), _prev(prev) {}
    SingleLinkedListNodeReference(const SingleLinkedListNodeReference<T> &ref) : _node(ref.node()), _prev(ref.prev()) {}

    SingleLinkedListNode<T> *node() const {
        return _node;
    }

    SingleLinkedListNode<T> *prev() const {
        return _prev;
    }
};

// Дескриптор односвязного списка: содержит начало, размер и основные методы управления списком
template <typename T>
class SingleLinkedList {
private:
    SingleLinkedListNode<T> *_first = nullptr;
    size_t _size = 0;
public:
    SingleLinkedList() = default;

    ~SingleLinkedList() {
        if (_first != nullptr) {
            delete _first;
        }
    }

    /**
     * Важно: добавление элемента происходит по значению! Т.е. все поля будут скопированы в новый блок памяти
     * Иначе мы получаем при малейшей оплошности утечку памяти в лучшем случае и доступ к чужим данным - в худшем.
     */
    SingleLinkedListNode<T> *attach(T *value) {
        return attach(new SingleLinkedListNode<T>(value));
    }

    /**
     * А вот здесь уже идет работа по указателю. Это позволяет ноды перемещать в пределах одного списка и между разными
     */
    SingleLinkedListNode<T> *attach(SingleLinkedListNode<T> *value) {
        value->next(_first);
        _size++;
        _first = value;

        return value;
    }

    /**
     * см. SingleLinkedListNode<T>::search()
     */
    SingleLinkedListNodeReference<T> search(const function<bool(const T &value)> &searchFn) const {
        return _first->search(searchFn);
    }

    /**
     * Первый элемент списка
     */
    SingleLinkedListNode<T> *first() const {
        return _first;
    }

    /**
     * Общее количество элементов, чтобы исключить полный обход для подсчета
     */
    size_t size() const {
        return _size;
    }

    /**
     * Функция удаления
     * @param value
     */
    void detach(SingleLinkedListNodeReference<T> &value) {
        auto node = value.node();

        if (value.prev() != nullptr) {
            // С момента создания ссылки на ноду:
            // 1. между ней и предыдущим объектом был вставлен другой
            // 2. предыдущий объект был удален
            // 3. текущий объект уже удален
            if (value.prev()->next() != value.node()) {
                throw logic_error("Предыдущий узел был изменен, продолжение работы невозможно");
            }

            value.prev()->next(node->next());
        } else {
            _first = node->next();
        }

        // Убиваем ссылку, чтобы гарантировать одномерность списка и защиту от ошибочных состояний
        node->next(nullptr);
        _size--;
    }
};

// генерация последовательных ID для удобства
static long idSeq = 0;

// автобус
struct Bus {
    long id;
    string regNo = string("");
    int routeNo = 0;
    string driverName = string("");
    Bus() {
        id = ++idSeq;
    }
    Bus(string regNo, int routeNo, string driverName) : regNo(std::move(regNo)), routeNo(routeNo), driverName(std::move(driverName)) {
        id = ++idSeq;
    }
    static Bus *createRandom() {
        auto bus = new Bus();
        // Генерация случайного госномера
        bus->regNo = string({
            (char)random('A', 'Z'),
            (char)random('A', 'Z'),
            (char)random('0', '9'),
            (char)random('0', '9'),
            (char)random('0', '9'),
            (char)random('A', 'Z'),
        });

        // Генерация случайного маршрута
        bus->routeNo = random(1, 100);

        // Имени и фамилии водителя
        static const char * names[] = {"Александр","Алексей","Егор","Иван","Игорь","Илья","Павел","Роман","Руслан","Сергей","Федор","Юрий","Ярослав"};
        static const char * secondNames[] = {"Иванов","Смирнов","Кузнецов","Попов","Васильев","Петров","Соколов","Михайлов","Новиков","Фёдоров","Морозов","Волков"};

        bus->driverName =
                string(names[random(0, (sizeof(names) / sizeof(char *)) - 1)])
                + " " +
                string(secondNames[random(0, (sizeof(secondNames) / sizeof(char *)) - 1)]);

        return bus;
    }
};

// Для удобства вывода на экран
ostream& operator <<(ostream &os, const Bus &bus) {
    return os << bus.id << ". " << bus.driverName << ", " << bus.regNo << ", № " << bus.routeNo;
}

int main () {
    // автопарк
    SingleLinkedList<Bus> busPark;
    // на маршруте
    SingleLinkedList<Bus> activeBus;

    // 5 случайных водителей
    for (int i = 0; i < 5; ++i) {
        busPark.attach(Bus::createRandom());
    }

    // короткая справка
    cout << "Usage: command[ args]" << endl;
    cout << "Type help to show available commands" << endl;

    char command[100];

    // основной цикл пользовательского ввода
    while (true) {
        cout << "> ";
        cin.getline(command, 100);

        // мы должны видеть что вводит скрипт для удобства проверки
        cout << command << endl;

        // разбираем команду на аргументы, разделенные пробелом
        // сначала идет операция
        char *op = strtok(command, " ");

        if (op == nullptr) {
            continue;
        }

        if (strcmp(op, "help") == 0) {
            // полная справка
            showUsage();
        } else if (strcmp(op, "exit") == 0) {
            // прощаемся и завершаем работу
            cout << "Good bye!" << endl;
            break;
        } else if (strcmp(op, "show") == 0) {
            // далее, обработчик каждой команды по-своему разбирает последующие аргументы
            char *type = strtok(nullptr, " ");

            if (type == nullptr) {
                cout << "Usage: show <park|active> - показать автобусы в автопарке/на маршруте" << endl;
                continue;
            }

            if (strcmp(type, "park") == 0) {
                cout << "Автобусы в автопарке:" << endl;
                // Бизнес-логика команды
                showStat(busPark);
            } else if (strcmp(type, "active") == 0) {
                cout << "Автобусы на маршруте:" << endl;
                // Бизнес-логика команды
                showStat(activeBus);
            }
        } else if (strcmp(op, "go") == 0 || strcmp(op, "stop") == 0) {
            // Бизнес-логика команды
            for(int i = 1;; i++) {
                char *idStr = strtok(nullptr, " ");

                if (idStr == nullptr) {
                    break;
                }

                char *endPtr = nullptr;
                auto id = strtol(idStr, &endPtr, 10);

                if (errno != 0 || endPtr == idStr) {
                    // сообщения об ошибке выводим в stderr
                    cerr << "Неверный аргумент #" << i << ": '" << idStr << "' не может быть преобразовано в число: " << strerror(errno)
                         << " (" << errno << ")" << endl;
                    continue;
                }

                if (strcmp(op, "go") == 0) {
                    const auto bus = moveBus(id, busPark, activeBus);
                    if (bus != nullptr) {
                        cout << "Отправлен на маршрут: " << *bus << endl;
                    } else {
                        // сообщения об ошибке выводим в stderr
                        cerr << "Автобус " << id << " не найден в автопарке" << endl;
                    }
                } else {
                    const auto bus = moveBus(id, activeBus, busPark);
                    if (bus != nullptr) {
                        cout << "Снят с маршрута: " << *bus << endl;
                    } else {
                        // сообщения об ошибке выводим в stderr
                        cerr << "Автобус " << id << " не найден на маршруте" << endl;
                    }
                }
            }
        } else {
            // сообщения об ошибке выводим в stderr
            cerr << "Команда '" << op << "' не существует" << endl << endl;
            showUsage();
            continue;
        }
    }
}

/**
 * Справки
 */
void showUsage() {
    cout << "Usage op[ arg1, arg2, ...]" << endl;
    cout << "Доступные команды:" << endl;
    cout << "help - показать эту справку" << endl;
    cout << "show <park|active> - показать автобусы в автопарке/на маршруте" << endl;
    cout << "go <id>[ id2, id3, ...] - вывести на маршрут" << endl;
    cout << "stop <id> - завершить работу автобуса и вернуться в автопарк" << endl;
    cout << "exit - завершить работу" << endl << endl;
}

/**
 * Перемещаем элемент по его ID из списка from в список to.
 */
const Bus *moveBus(long id, SingleLinkedList<Bus> &from, SingleLinkedList<Bus> &to) {
    // Находим элемент
    auto busRef = from.search([&id] (auto v) {
        return v.id == id;
    });

    // Если не нашли - то вернем nullptr
    if (busRef.node() == nullptr) {
        return nullptr;
    }

    // отсоединяем элемент из одного списка
    // важно что сам элемент при этом не удаляется и остается доступен
    from.detach(busRef);
    // и добавляем в другой
    to.attach(busRef.node());

    return busRef.node()->get();
}

/**
 * вывод содержимого списка
 * @param list
 */
void showStat(const SingleLinkedList<Bus> &list) {
    cout << "Всего автобусов " << list.size() << endl;
    auto node = list.first();

    // простейший обход односвязного списка
    while (node != nullptr) {
        const Bus *bus = node->get();
        cout << *bus << endl;
        node = node->next();
    }
}

/**
 * Генерируем случаное целое число от и до включительно
 * rand считаеся "плохим" генератором, но нас устраивает
 */
int random(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}
