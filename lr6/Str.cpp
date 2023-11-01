#include <cstring>
#include "Str.h"

static const char *empty = "";
static StrRef emptyRef = {empty, 0, 0};

// Пустой конструктор для Str s;
Str::Str() {
    ref = &emptyRef;
}

// Конструктор для преобразования из char *
// Str s = "a";
// Str s("a", true); - предпочтительно
Str::Str(const char *chars, bool _static) {
    auto l = strlen(chars);
    if (!_static) {
        // Если строка в динамической памяти, то мы должны ее скопировать
        // Для этого выделяем буфер размером с StrRef + длина строки (с учетом NUL-байта)
        // Нужно будет учесть нюанс, если мы будем использовать для класса перегрузку оператора new:
        // перегруженный оператор не вызовется по умолчанию, это придется сделать явно.
        // Есть два преимущества этого подхода:
        // 1. В 2 раза меньше обращений к менеджеру памяти, в целом меньший расход памяти, за счет накладных расходов на ее выделение
        // 2. Проще реализация деструктора: нужно лишь удалить объект, если он больше не нужен. В противнос случае пришлось бы различать статические и динамические строки и уничтожать их по-разному.
        auto refBuffer = new char[sizeof (StrRef) / sizeof (char) + l + 1];
        ref = (StrRef *)refBuffer;
        // Начало строки - сразу после структуры
        ref->cStr = refBuffer + sizeof(StrRef) / sizeof (char);
        memcpy((void *)ref->cStr, chars, l + 1);
    } else {
        // Если строка статическая, то и копировать ее не обязательно
        ref = new StrRef();
        ref->cStr = chars;
    }

    ref->length = (int)l;
    ref->refcount = 1;
}
// Копирующий конструктор
// Str s(s1);
Str::Str(Str const &strRef) {
    ref = strRef.ref;
    addRef();
}

void Str::addRef() {
    if (ref != &emptyRef) {
        ref->refcount++;
    }
}

// Приватный конструктор с конкатенацией, для оператора +
Str::Str(const char *chars1, const char *chars2) {
    // Очень похож на обычный констуктор, только здесь всегда строка динамическая
    auto l1 = strlen(chars1);
    auto l2 = strlen(chars2);
    auto l = l1 + l2;
    auto refBuffer = new char[sizeof (StrRef) + l + 1];
    ref = (StrRef *)refBuffer;
    ref->length = (int)l;
    ref->refcount = 1;
    ref->cStr = refBuffer + sizeof(StrRef);
    memcpy((void *)ref->cStr, chars1, l1);
    memcpy((void *)(ref->cStr + l1), chars2, l2 + 1);
}

// Деструктор
Str::~Str() {
    destroy();
}

// Получает C-style строку
const char *Str::cStr() const {
    return ref->cStr;
}

// Конкатенация
// Str s = a + b;
// Str s = a + "b";
// Str s = "a" + "b";
Str Str::operator+(Str const &str2) const {
    Str result(this->cStr(), str2.cStr());

    // Возвращаем по значению. Оптимизатор должен все вызовы конструкторов и присваиваний свести к простому возврату ссылки StrRef
    return result;
}

// Оператор присваивания
// Str s = a;
// Str s = "a";
Str &Str::operator=(const Str &str2) {
    if (this == &str2) {
        return *this;
    }

    // Сначала освободим память от предыдущего значения
    destroy();
    ref = str2.ref;
    addRef();

    return *this;
}

// Оператор сравнения
bool Str::operator==(Str &str2) const {
    return strcmp(this->cStr(), str2.cStr()) == 0;
}

// Оператор сравнения
bool Str::operator!=(Str &str2) const {
    return !(*this == str2);
}

// Реализация деструктора.
// Уменьшаем счетчик ссылок, освобождаем память если 0
void Str::destroy() {
    if (ref == &emptyRef) {
        return;
    }

    ref->refcount--;

    if (ref->refcount > 0) {
        return;
    }

    delete ref;
    ref = &emptyRef;
}
