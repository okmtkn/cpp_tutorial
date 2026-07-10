# モダンC++ & CMake チュートリアル（機械系高専生向け）

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
| 0 | [docs/00_setup.md](docs/00_setup.md) | 環境構築（WinLibs GCC / CMake / Ninja / VS Code） |
| 1 | [docs/01_first_build.md](docs/01_first_build.md) | はじめてのビルド — CMakeLists.txt の基本と VS Code での実行 |
| 2 | [docs/02_from_c_to_cpp.md](docs/02_from_c_to_cpp.md) | CからC++へ — cout / string / vector / auto / 範囲for |
| 3 | [docs/03_functions.md](docs/03_functions.md) | 関数の進化 — オーバーロード / 参照渡し / 構造化束縛 |
| 4 | [docs/04_classes.md](docs/04_classes.md) | クラス — コンストラクタ / メンバ関数 / 演算子オーバーロード |
| 5 | [docs/05_stl.md](docs/05_stl.md) | STLとラムダ式 — algorithm / map / ファイル入出力とCSV処理 |
| 6 | [docs/06_raii_smart_ptr.md](docs/06_raii_smart_ptr.md) | RAIIとスマートポインタ — unique_ptr / shared_ptr / 多態性 |
| 7 | [docs/07_templates_optional.md](docs/07_templates_optional.md) | テンプレート入門と std::optional |
| 8 | [docs/08_cmake_project.md](docs/08_cmake_project.md) | CMake実践 — 複数ファイル分割と静的ライブラリ |
| 9 | [docs/09_external_lib_test.md](docs/09_external_lib_test.md) | 外部ライブラリ(FetchContent)とテスト(CTest) |
| 10 | [docs/10_final_project.md](docs/10_final_project.md) | 総合演習 — ばね・質量・ダンパ系のシミュレーション |

想定学習時間：1章あたり 1〜2 コマ（90分）程度、全体で 15〜20 時間。

## フォルダ構成

```
modern_c++_tutorial/
├── README.md            ← このファイル
├── CMakeLists.txt       ← examples をまとめてビルドするための設定
├── docs/                ← チュートリアル本文（0章から順に読む）
├── examples/            ← 各章の完成コード
│   ├── ch01_hello/
│   ├── ...
│   ├── ch08_cmake_lib/
│   ├── ch09_external/   ← 単独プロジェクト（この folder を別途開く）
│   └── ch10_project/    ← 単独プロジェクト（この folder を別途開く）
└── work/                ← 自分の練習用（自由に作ってOK）
```

- **このフォルダを VS Code で開くと、1〜8章のサンプルをまとめてビルド・実行できます**
  （やり方は 1 章で説明します）。
- 9・10章は「独立した実プロジェクト」の体裁で作ってあるため、それぞれのフォルダを
  単独で VS Code で開いて進めます。

## 必要なもの

- Windows 10/11 の PC（**管理者権限は不要**な手順で構成しています）
- インターネット接続（ツールのダウンロードと 9 章の外部ライブラリ取得に使用）

それでは [docs/00_setup.md](docs/00_setup.md) から始めましょう。
