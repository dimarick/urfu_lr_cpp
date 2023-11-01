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
4. `build/lr5_file2 /home/dima/lr_cpp/lr5/test.txt /tmp/test2.txt && cat /tmp/test2.txt`
5, 6. `build/lr5_file3 "secret password" lr_cpp/lr5/test.txt /tmp/test2.txt && build/lr5_file3 "secret password" /tmp/test2.txt /tmp/test3.txt && cat /tmp/test3.txt`

Лабораторная работа 5
`lr6_main`
