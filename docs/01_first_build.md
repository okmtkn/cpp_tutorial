# 第1章 はじめてのビルド

この章では「Hello, C++!」を CMake + Ninja でビルドし、VS Code から実行・デバッグします。
最初の1回はターミナルで手動ビルドして仕組みを理解し、その後 VS Code のボタン操作に移ります。

## 1.1 プロジェクトフォルダを作る

このチュートリアルのフォルダ内に `work\ch01` フォルダを作り、
**そのフォルダを VS Code で開いてください**（メニュー: ファイル → フォルダーを開く）。

作るファイルは2つだけです。

```
work\ch01\
├── main.cpp        ← C++ のソースコード
└── CMakeLists.txt  ← ビルドの設計図
```

## 1.2 main.cpp を書く

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, C++!" << std::endl;
    return 0;
}
```

C言語との違いはあとの章でじっくりやります。今は
「`printf` の代わりに `std::cout <<` を使う」とだけ思ってください。

## 1.3 CMakeLists.txt を書く

ファイル名は **正確に** `CMakeLists.txt` です（大文字小文字も含めて）。

```cmake
# CMake 自体の最低バージョンを宣言（お約束の1行目）
cmake_minimum_required(VERSION 3.21)

# プロジェクト名と使用言語（CXX = C++）
project(hello CXX)

# C++17 を使う宣言
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 「main.cpp から hello.exe を作れ」という意味
add_executable(hello main.cpp)
```

覚えるべき本質はたった1行、`add_executable(実行ファイル名 ソースファイル...)` です。
残りは毎回同じ「お約束」なので、コピペで構いません。

## 1.4 ターミナルからビルドしてみる

VS Code でターミナルを開き（`` Ctrl+@ `` または メニュー: ターミナル → 新しいターミナル）、
次の2コマンドを実行します。

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

意味は次のとおりです。

| コマンド | 意味 |
|----------|------|
| `cmake -S . -B build -G Ninja` | **configure（構成）**: 現在のフォルダ(`-S .`)の CMakeLists.txt を読み、`build` フォルダ(`-B build`)に Ninja 用のビルド手順を生成する |
| `cmake --build build` | **build**: 生成した手順に従って Ninja がコンパイルを実行する |

成功すると `build\hello.exe` ができています。実行してみましょう。

```powershell
.\build\hello.exe
```

```
Hello, C++!
```

> **ポイント: out-of-source ビルド**
> 生成物はすべて `build` フォルダの中にできます。ソースコードと生成物が混ざらないので、
> `build` フォルダごと消せばいつでもクリーンな状態に戻せます。
> Git で管理するときも `build/` を `.gitignore` に書くだけで済みます。

> **ポイント: configure は最初の1回だけ**
> 2回目以降、コードを書き換えたら `cmake --build build` だけでOKです。
> CMakeLists.txt を書き換えたときは configure からやり直します
> （実は build コマンドが自動で再 configure してくれます）。

## 1.5 VS Code (CMake Tools) でビルド・実行する

毎回コマンドを打つ代わりに、CMake Tools 拡張を使うとボタン操作でビルドできます。

### キット（コンパイラ）の選択

1. `Ctrl+Shift+P` でコマンドパレットを開き、「**CMake: キットの選択**」
   （CMake: Select a Kit）を実行
2. 一覧から **GCC x.x.x ... mingw64** のようなキットを選ぶ
   - 出てこない場合は「キットの検索」(Scan for kits) を実行してから再度選択

キットを選ぶと自動で configure が走り、下の「出力」パネルにログが流れます。

### ビルドと実行

画面**下部のステータスバー**に CMake Tools の操作ボタンが並んでいます。

- **ビルド（歯車アイコン Build）** … `cmake --build` と同じ。ショートカットは `F7`
- **実行（▶ アイコン）** … ビルドして実行。ショートカットは `Shift+F5`
- **デバッグ（虫アイコン）** … デバッガ(gdb)付きで実行。`Ctrl+F5`

▶ ボタンを押して、ターミナルに `Hello, C++!` が出れば成功です。

> **Ninja が使われているか確認したい場合:**
> このチュートリアルのルートには `.vscode/settings.json` で `"cmake.generator": "Ninja"`
> を指定してあります。自分のプロジェクトで指定したい場合は同じ設定を書くか、
> configure のログに `-G Ninja` が出ているか確認してください。

## 1.6 デバッガを使ってみる

デバッガは printf デバッグから卒業するための道具です。

1. `main.cpp` の `std::cout` の行の**行番号の左**をクリックして赤丸（ブレークポイント）を付ける
2. ステータスバーの**虫アイコン**（またはコマンドパレットで「CMake: デバッグ」）を実行
3. その行で実行が一時停止し、変数の中身が左パネルに表示される
4. 上部のボタンで「ステップオーバー(F10)」「続行(F5)」などを操作できる

今は止まる場所が1行しかないので面白くありませんが、
プログラムが複雑になる後の章で威力を発揮します。使い方だけ覚えておいてください。

## 1.7 チュートリアル全体のサンプルをビルドする

このチュートリアルの**ルートフォルダ**（README.md がある場所）を VS Code で開くと、
1〜8章の全サンプルコードをまとめてビルドできます。

1. ルートフォルダを開く → キットを選択 → 自動で configure
2. `F7` で全ターゲットをビルド
3. 実行したいプログラムの選択: ステータスバーの
   **「起動ターゲットを選択」（▶の隣の [プログラム名] 部分）** をクリックし、
   例えば `ch01_hello` を選んで ▶ で実行

各章のサンプルは `ch02_string_basics` のように「章番号_名前」のターゲット名になっています。

## 1.8 演習

1. メッセージを自分の名前入りに変えてビルド・実行してみましょう。
   日本語を出力して文字化けしたら、第0章 0.9 の `chcp 65001` を試してください。
2. `main.cpp` をコピーして `goodbye.cpp` を作り、CMakeLists.txt に
   `add_executable(goodbye goodbye.cpp)` を追記して、**1つのプロジェクトから
   2つの実行ファイル**を作ってみましょう。
   （VS Code の起動ターゲットを切り替えて両方実行できれば成功です）
3. `build` フォルダを丸ごと削除してから、もう一度ビルドしてみましょう。
   configure からやり直せば元通りになることを確認してください。

完成版コード: [`examples/ch01_hello/`](../examples/ch01_hello/)

次章: [第2章 CからC++へ](02_from_c_to_cpp.md)
