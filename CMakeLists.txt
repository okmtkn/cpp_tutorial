# チュートリアル全体（1〜8章）の例題をまとめてビルドするための CMakeLists.txt
# 書き方の意味は docs/01_first_build.md と docs/08_cmake_project.md で解説します。
cmake_minimum_required(VERSION 3.21)
project(modern_cpp_tutorial CXX)

# C++17 を必須にする
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 警告を多めに出す（初学者ほど警告に助けられます）
add_compile_options(-Wall -Wextra)

add_subdirectory(examples/ch01_hello)
add_subdirectory(examples/ch02_from_c)
add_subdirectory(examples/ch03_functions)
add_subdirectory(examples/ch04_classes)
add_subdirectory(examples/ch05_stl)
add_subdirectory(examples/ch06_smart_ptr)
add_subdirectory(examples/ch07_templates)
add_subdirectory(examples/ch08_cmake_lib)

# ch09_external と ch10_project は独立プロジェクトのため、ここには含めません。
# それぞれのフォルダを VS Code で単独で開いてください。
