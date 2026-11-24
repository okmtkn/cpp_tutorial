# 第2章: CからC++へ
cmake_minimum_required(VERSION 3.21)
project(ch02_from_c CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 警告を多めに出す（初学者ほど警告に助けられます）
add_compile_options(-Wall -Wextra)

add_executable(ch02_hello_io hello_io.cpp)
add_executable(ch02_string_basics string_basics.cpp)
add_executable(ch02_vector_basics vector_basics.cpp)
add_executable(ch02_range_for range_for.cpp)

# 演習の解答例
add_executable(ch02_sol_temp_stats solutions/temp_stats.cpp)
add_executable(ch02_sol_reverse_names solutions/reverse_names.cpp)
add_executable(ch02_sol_line_collector solutions/line_collector.cpp)
