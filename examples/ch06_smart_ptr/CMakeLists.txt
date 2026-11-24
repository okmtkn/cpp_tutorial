# 第6章: RAIIとスマートポインタ
cmake_minimum_required(VERSION 3.21)
project(ch06_smart_ptr CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 警告を多めに出す（初学者ほど警告に助けられます）
add_compile_options(-Wall -Wextra)

add_executable(ch06_raii raii_demo.cpp)
add_executable(ch06_unique_ptr unique_ptr_basics.cpp)
add_executable(ch06_shapes shapes_poly.cpp)
add_executable(ch06_shared_ptr shared_ptr_basics.cpp)

# 演習の解答例
add_executable(ch06_sol_shapes solutions/shapes_exercise.cpp)
