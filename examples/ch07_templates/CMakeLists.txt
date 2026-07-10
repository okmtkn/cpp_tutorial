# 第7章: テンプレート入門と std::optional
cmake_minimum_required(VERSION 3.21)
project(ch07_templates CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ch07_func_template func_template.cpp)
add_executable(ch07_class_template class_template.cpp)
add_executable(ch07_optional optional_demo.cpp)

# 演習の解答例
add_executable(ch07_sol_lerp solutions/lerp.cpp)
add_executable(ch07_sol_find solutions/find_first_ge.cpp)
add_executable(ch07_sol_temp_at solutions/temp_at.cpp)
