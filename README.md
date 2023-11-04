Статус
======
[![Демо работоспособности всех утилит](https://github.com/dimarick/urfu_lr_cpp/actions/workflows/demo.yml/badge.svg)](https://github.com/dimarick/urfu_lr_cpp/actions/workflows/demo.yml)

Сборка
======

`cd build && cmake .. && make -j 8; cd ..`

Запуск
======
Лабораторная работа 1
1. `build/lr1_power 3 39`
2. `build/lr1_ptr`
2. `build/lr1_swp`

Лабораторная работа 2
1. `build/lr2_matrix`

Лабораторная работа 3
1.`build/lr3_sort`
2.`build/lr3_sort2`

Лабораторная работа 4
1.`build/lr4_strings_static`
2.`build/lr4_strings_malloc`
3.`build/lr4_strings_calloc`
4.`build/lr4_strings_array`

Лабораторная работа 5
1, 2, 3. `(echo "lr5/file1.cpp" && echo search) | build/lr5_file1 && cat matches.txt`
4. `build/lr5_file2 lr5/test.txt /tmp/test2.txt && cat /tmp/test2.txt`
5, 6. `build/lr5_file3 "secret password" lr5/test.txt /tmp/test2.txt && build/lr5_file3 "secret password" /tmp/test2.txt /tmp/test3.txt && cat /tmp/test3.txt`

Лабораторная работа 6
`lr6_main`

Лабораторная работа 8
1. `echo "test" | build/lr8_strings`
2. `build/lr8_template1 && build/lr8_template2 && build/lr8_template3`
3. `build/lr8_encrypt "secret password" lr5/test.txt /tmp/test2.txt && build/lr5_file3 "secret password" /tmp/test2.txt /tmp/test3.txt && cat /tmp/test3.txt`
4. Неясно о каких именно методах идет речь

Лабораторная работа 9

1-2. См. п2. Лабораторной работы 8

3. `build/lr9_train_sort`