# 第3章: 関数の進化
cmake_minimum_required(VERSION 3.21)
project(ch03_functions CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ch03_overload overload.cpp)
add_executable(ch03_pass_by pass_by.cpp)
add_executable(ch03_pair_binding pair_binding.cpp)

# 演習の解答例
add_executable(ch03_sol_trapezoid solutions/trapezoid.cpp)
add_executable(ch03_sol_mean_stddev solutions/mean_stddev.cpp)
