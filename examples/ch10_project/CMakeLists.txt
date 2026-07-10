# 第10章: 総合演習 — ばね・質量・ダンパ系シミュレータ
# ※ このフォルダを VS Code で単独で開いてビルドしてください。
cmake_minimum_required(VERSION 3.21)
project(ch10_project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

# ---- シミュレーションライブラリ ----
add_library(sim STATIC
    src/model.cpp
    src/integrator.cpp
)
target_include_directories(sim PUBLIC include)

# ---- アプリ ----
add_executable(sim_app apps/main.cpp)
target_link_libraries(sim_app PRIVATE sim)

# ---- テスト ----
enable_testing()

add_executable(test_integrator tests/test_integrator.cpp)
target_link_libraries(test_integrator PRIVATE sim)
add_test(NAME integrator COMMAND test_integrator)
