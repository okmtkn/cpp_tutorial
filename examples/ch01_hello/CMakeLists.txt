# 第1章: はじめてのビルド
# ※ このフォルダを VS Code で単独で開いてビルドしてください。
cmake_minimum_required(VERSION 3.21)
project(ch01_hello CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 警告を多めに出す（初学者ほど警告に助けられます）
add_compile_options(-Wall -Wextra)

add_executable(ch01_hello main.cpp)

# 演習2: 1つのプロジェクトから複数の実行ファイルを作る
add_executable(ch01_goodbye goodbye.cpp)
