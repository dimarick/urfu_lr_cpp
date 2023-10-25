#include <stdio.h>

//Выводим адрес указателя, значение по указателю, адрес переменной с указателем
#define DEBUG_PTR(x) printf("x=%p, *x=%d, &x=%p\n", x, *x, &x);

int main (void) {
    int array[] = {100, 200, 300};
    int *ptr1, *ptr2;
    ptr1 = array;
    ptr2 = &array[2];
    // ptr1 указывает на начало массива, т.е. на первый его элемент
    DEBUG_PTR(ptr1);
    // прибавление X к указателю, увеличит значение адреса на X*sizeof(*ptr1), в данном случае, на 4 (размер int)
    ptr1++;
    DEBUG_PTR(ptr1);
    DEBUG_PTR(ptr2);
    // преинкремент здесь сработает так же, как и постинкремент
    ++ptr2;
    // разность между указателями равна 2, хотя адреса отличаются на 8
    printf("ptr2 - ptr1=%u\n", (unsigned int)(ptr2 - ptr1));
}