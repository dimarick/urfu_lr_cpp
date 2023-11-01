//
// Created by dima on 30.10.23.
//

#ifndef LR_STR_H
#define LR_STR_H

// объект строки, чтобы избегать лишнего расхода памяти, для одной и той же строки мы будем использовать один и тот же объект StrRef
struct StrRef {
    // ссылка на строку
    const char *cStr = nullptr;
    // длина строки
    int length = 0;
    // счетчик ссыллок. Когда становится 0, объект можно уничтожить
    short refcount = 0;
};

class Str {
private:
    // Объект строки содержит лишь 1 указатель, поэтому он очень легковесный и операции с ним очень хорошо оптимизируются
    StrRef *ref;
public:
    // Пустой конструктор для Str s;
    Str();
    // Конструктор для преобразования из char *
    // Str s = "a";
    // Str s("a", true); - предпочтительно
    Str(const char *chars, bool _static = false);
    // Копирующий конструктор
    // Str s(s1);
    Str(Str const &strRef);
    // Деструктор
    ~Str();
    // Получает C-style строку
    const char *cStr() const;

    // Конкатенация
    // Str s = a + b;
    // Str s = a + "b";
    // Str s = "a" + "b";
    Str operator+(Str const &str2) const;

    // Оператор присваивания
    // Str s = a;
    // Str s = "a";
    Str &operator=(Str const &str2);

    // Операторы сравнения
    bool operator==(Str &str2) const;
    bool operator!=(Str &str2) const;

private:
    // конструктор с конкатенацией
    Str(const char *chars1, const char *chars2);
    // реализация деструктора
    void destroy();
    void addRef();
};


#endif //LR_STR_H
