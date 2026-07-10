# 第1章: はじめてのビルド
# このファイル単体でも、ルートの CMakeLists.txt からでもビルドできるようにしてあります。
cmake_minimum_required(VERSION 3.21)
project(ch01_hello CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ch01_hello main.cpp)

# 演習2: 1つのプロジェクトから複数の実行ファイルを作る
add_executable(ch01_goodbye goodbye.cpp)
