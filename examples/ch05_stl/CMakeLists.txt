# 第5章: STLとラムダ式
cmake_minimum_required(VERSION 3.21)
project(ch05_stl CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 警告を多めに出す（初学者ほど警告に助けられます）
add_compile_options(-Wall -Wextra)

add_executable(ch05_algorithms algorithms.cpp)
add_executable(ch05_lambda lambda.cpp)
add_executable(ch05_map map_table.cpp)
add_executable(ch05_fstream fstream_basics.cpp)

add_executable(ch05_csv_stats csv_stats.cpp)
# CSVファイルのあるフォルダの絶対パスをマクロ DATA_DIR として埋め込む。
# こうすると exe をどこから実行してもデータファイルが見つかる（第8章参照）。
target_compile_definitions(ch05_csv_stats
    PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

# 演習の解答例
add_executable(ch05_sol_csv solutions/csv_exercise.cpp)
target_compile_definitions(ch05_sol_csv
    PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
