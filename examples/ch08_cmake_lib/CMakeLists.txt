# 第8章: 複数ファイル分割と静的ライブラリ
cmake_minimum_required(VERSION 3.21)
project(ch08_cmake_lib CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (1) 静的ライブラリ numlib を作る
add_library(numlib STATIC
    src/integrate.cpp
    src/statistics.cpp
)

# (2) numlib のヘッダの場所を教える
#     PUBLIC = numlib 自身にも、numlib を使う側にも適用される
target_include_directories(numlib PUBLIC include)

# (3) 実行ファイルを作り、numlib をリンクする
add_executable(ch08_app apps/main.cpp)
target_link_libraries(ch08_app PRIVATE numlib)

# 演習2: 2つ目の実行ファイル
add_executable(ch08_integral_table apps/integral_table.cpp)
target_link_libraries(ch08_integral_table PRIVATE numlib)
