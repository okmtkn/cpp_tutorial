# 第4章: クラス
cmake_minimum_required(VERSION 3.21)
project(ch04_classes CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ch04_vec2 vec2.cpp)
add_executable(ch04_specimen specimen.cpp)
add_executable(ch04_particle particle.cpp)

# 演習の解答例
add_executable(ch04_sol_vec3 solutions/vec3.cpp)
add_executable(ch04_sol_drag solutions/particle_drag.cpp)
