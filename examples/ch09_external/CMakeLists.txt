# 第9章: 外部ライブラリ (FetchContent) とテスト (CTest)
# ※ このフォルダを VS Code で単独で開いてビルドしてください。
# ※ 初回 configure 時に fmt をダウンロードするため、ネット接続が必要です。
cmake_minimum_required(VERSION 3.21)
project(ch09_external CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

# ---- 外部ライブラリ: fmt を FetchContent で取得 ----
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.1.4   # バージョン固定が重要（再現性）
)
FetchContent_MakeAvailable(fmt)

# ---- 自作ライブラリ ----
add_library(mathutil STATIC src/mathutil.cpp)
target_include_directories(mathutil PUBLIC src)

# ---- アプリ ----
add_executable(report src/main.cpp)
target_link_libraries(report PRIVATE mathutil fmt::fmt)

# ---- テスト ----
enable_testing()

add_executable(test_mathutil tests/test_mathutil.cpp)
target_link_libraries(test_mathutil PRIVATE mathutil)

# 「test_mathutil を実行して終了コード 0 なら合格」というテストを登録
add_test(NAME mathutil COMMAND test_mathutil)
