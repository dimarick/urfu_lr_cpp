cmake_minimum_required(VERSION 3.16)
project(lr)

set(CMAKE_CXX_STANDARD 11)

add_executable(lr1_power lr1/power.cpp)
add_executable(lr1_ptr lr1/ptr.cpp)
add_executable(lr1_swp lr1/swp.cpp)
add_executable(lr2_matrix lr2/matrix.cpp)