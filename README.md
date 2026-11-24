# モダンC++ チュートリアル（機械系高専生向け）

C言語を習得した機械系高専生を対象に、**モダンC++（C++17）** と **CMake** によるビルドを、
**VS Code + MinGW(GCC) + Ninja** の環境で学ぶ自習用チュートリアルです。

すべての教材は Markdown で書かれているので、VS Code 上で読みながら進められます
（`.md` ファイルを開いて `Ctrl+Shift+V` でプレビュー表示できます）。

## 進め方

1. **`docs/00_setup.md`** から順番に読み進めてください。
2. 各章には短いサンプルコードと章末演習があります。
3. 完成版のコードは `examples/` に入っています。手を動かして写経 → 改造するのがおすすめです。
4. 演習の解答例は各章の `examples/chXX_*/solutions/` にあります。**まず自力で解いてから**見てください。

## 目次

| 章 | ファイル | 内容 |
|----|---------|------|
| 0 | [docs/00_setup.md](docs/00_setup.md) | 環境構築（WinLibs GCC / CMake / Ninja / Git / VS Code） |
| 1 | [docs/01_first_build.md](docs/01_first_build.md) | はじめてのビルド — CMakeLists.txt の基本と VS Code での実行 |
| 2 | [docs/02_from_c_to_cpp.md](docs/02_from_c_to_cpp.md) | CからC++へ — cout / string / vector / auto / 範囲for / 命名規則 |
| 3 | [docs/03_functions.md](docs/03_functions.md) | 関数の進化 — オーバーロード / 参照渡し / 構造化束縛 |
| 4 | [docs/04_classes.md](docs/04_classes.md) | クラス — コンストラクタ / メンバ関数 / 演算子オーバーロード |
| 5 | [docs/05_stl.md](docs/05_stl.md) | STLとラムダ式 — algorithm / map / ファイル入出力とCSV処理 |
| 6 | [docs/06_raii_smart_ptr.md](docs/06_raii_smart_ptr.md) | RAIIとスマートポインタ — unique_ptr / shared_ptr / 多態性 |
| 7 | [docs/07_templates_optional.md](docs/07_templates_optional.md) | テンプレート入門と std::optional |
| 8 | [docs/08_cmake_project.md](docs/08_cmake_project.md) | CMake実践 — 複数ファイル分割と静的ライブラリ |
| 9 | [docs/09_external_lib_test.md](docs/09_external_lib_test.md) | 外部ライブラリ(FetchContent)とテスト(CTest) |
| 10 | [docs/10_final_project.md](docs/10_final_project.md) | 総合演習 — ばね・質量・ダンパ系のシミュレーション |


## フォルダ構成

```
cpp_tutorial/
├── README.md            ← このファイル
├── docs/                ← チュートリアル本文（0章から順に読む）
├── examples/            ← 各章の完成コード（章ごとに独立したプロジェクト）
│   ├── ch01_hello/
│   ├── ...
│   ├── ch09_external/
│   └── ch10_project/
└── work/                ← 自分の練習用（自由に作ってOK）
```

- 各章のサンプルは**章のフォルダを VS Code で単独で開いて**ビルド・実行します
  （やり方は 1 章で説明します）。

## 必要なもの

- Windows 10/11 の PC
- インターネット接続（ツールのダウンロードと 9 章の外部ライブラリ取得に使用）
- このフォルダを**日本語・スペースを含まないパス**（例: `C:\dev\cpp_tutorial`）に置くこと
  （詳しくは 0 章）

それでは [docs/00_setup.md](docs/00_setup.md) から始めましょう。
