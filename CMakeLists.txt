cmake_minimum_required(VERSION 3.16)
project(lr)

set(CMAKE_CXX_STANDARD 14)

set(RELEASE_FLAGS "-Wall -O2 -marc=native -mtune=native")
set(DEBUG_FLAGS "-Wall -O0 -ggdb3")

add_executable(lr1_power lr1/power.cpp)
add_executable(lr1_ptr lr1/ptr.cpp)
add_executable(lr1_swp lr1/swp.cpp)
add_executable(lr2_matrix lr2/matrix.cpp)
add_executable(lr3_sort lr3/sort.cpp)
add_executable(lr3_sort2 lr3/sort2.cpp)
add_executable(lr4_strings_static lr4/strings.cpp lr4/strings_1static.cpp)
add_executable(lr4_strings_malloc lr4/strings.cpp lr4/strings_2malloc.cpp)
add_executable(lr4_strings_calloc lr4/strings.cpp lr4/strings_3calloc.cpp)
add_executable(lr4_strings_array lr4/strings.cpp lr4/strings_4array.cpp)
add_executable(lr5_file1 lr5/file1.cpp)
add_executable(lr5_file2 lr5/file2.cpp)
add_executable(lr5_file3 lr5/file3.cpp)
add_executable(lr6_main lr6/Str.cpp lr6/main.cpp)
add_executable(lr8_strings lr6/Str.cpp lr8/strings.cpp)
add_executable(lr8_template1 lr8/template1.cpp)
add_executable(lr8_template2 lr8/template2.cpp)
add_executable(lr8_template3 lr8/template3.cpp)
add_executable(lr8_encrypt lr8/encrypt.cpp)
add_executable(lr9_train_sort lr9/train_sort.cpp)
add_executable(lr10_bus_park lr10/bus_park.cpp)