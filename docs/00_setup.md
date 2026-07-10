# 第0章 環境構築

この章では、C++ の開発環境を **管理者権限なし** で Windows に構築します。
インストーラは使わず、基本的に「zip を展開して PATH を通す」だけで完了します。

## 0.1 これから入れるツールの役割

| ツール | 役割 |
|--------|------|
| **GCC (MinGW-w64 / WinLibs)** | コンパイラ。C++ のソースコードを実行ファイル(.exe)に変換する |
| **CMake** | ビルド設定ツール。「どのファイルをどうコンパイルするか」を記述・管理する |
| **Ninja** | ビルド実行ツール。CMake が生成した手順に従って高速にビルドする |
| **VS Code** | エディタ。コードを書き、ビルド・実行・デバッグの操作もここから行う |

関係を図にするとこうなります。

```
あなたが書くもの:  main.cpp （C++コード）  +  CMakeLists.txt （ビルドの設計図）
                         │
        CMake が読んで Ninja 用のビルド手順を生成
                         │
        Ninja が GCC を呼び出してコンパイル・リンク
                         │
                    main.exe （実行ファイル）
```

C言語の授業で `gcc main.c` と直接コンパイルしていたのに比べると大げさに見えますが、
ファイルが数十個になっても、ライブラリを使っても、**同じ操作でビルドできる**のが
CMake を使う理由です。実務や研究のC++プロジェクトはほぼ CMake で管理されています。

## 0.2 ツールを置くフォルダを作る

エクスプローラーで `C:\dev` フォルダを作ってください（場所は好みですが、
**日本語やスペースを含まないパス**にしてください。トラブルの元です）。

最終的に次の構成にします。

```
C:\dev\
├── mingw64\   ← GCC コンパイラ一式
├── cmake\     ← CMake
└── ninja\     ← Ninja
```

## 0.3 GCC（WinLibs 版 MinGW-w64）の導入

1. https://winlibs.com/ を開く
2. **「UCRT runtime」** の欄から、最新リリースの **「Win64」用 Zip アカイブ**を
   ダウンロードする
   - 「GCC 14.x.x + LLVM/Clang/... + MinGW-w64 (UCRT) - release N」のような表記の中の
     **Zip archive** リンクです（7-Zip archive でも中身は同じです）
   - 「without LLVM/Clang/LLD/LLDB」版で十分です（サイズが小さい）
3. ダウンロードした zip を展開すると `mingw64` というフォルダが出てくるので、
   `C:\dev\mingw64` になるように移動する
   - `C:\dev\mingw64\bin\g++.exe` が存在すればOK

> **メモ:** WinLibs には C/C++ コンパイラ（gcc / g++）に加えて、
> デバッガ（gdb）も含まれています。VS Code でのデバッグに使います。

## 0.4 CMake の導入

1. https://cmake.org/download/ を開く
2. 「Binary distributions」から **Windows x64 ZIP**（`cmake-x.x.x-windows-x86_64.zip`）を
   ダウンロードする
3. 展開して出てくるフォルダの中身を `C:\dev\cmake` に置く
   - `C:\dev\cmake\bin\cmake.exe` が存在すればOK

## 0.5 Ninja の導入

1. https://github.com/ninja-build/ninja/releases を開く
2. 最新リリースの **`ninja-win.zip`** をダウンロードする
3. 中身は `ninja.exe` 1個だけなので、`C:\dev\ninja\ninja.exe` として置く

## 0.6 PATH を通す

「PATH を通す」とは、ターミナルでコマンド名（例: `g++`）を打ったときに
Windows が実行ファイルを探しに行くフォルダの一覧に登録することです。

**管理者権限が不要な「ユーザー環境変数」に設定します。**

1. `Windows キー` を押して「**環境変数**」と入力し、
   「**アカウントの環境変数を編集**」を開く（「システム環境変数の編集」ではない方）
2. 上側の「〜のユーザー環境変数」の一覧から **Path** を選び「編集」
3. 「新規」で次の3行を1つずつ追加する

   ```
   C:\dev\mingw64\bin
   C:\dev\cmake\bin
   C:\dev\ninja
   ```

4. 「OK」で全部のダイアログを閉じる

> **注意:** PATH の変更は、**変更後に新しく開いたターミナルから**有効になります。
> すでに開いているターミナルや VS Code は一度閉じて開き直してください。

## 0.7 動作確認

`Windows キー` →「powershell」と入力して **新しく** PowerShell を開き、
次の4つのコマンドを実行してください。それぞれバージョン番号が表示されれば成功です。

```powershell
g++ --version
gcc --version
cmake --version
ninja --version
```

`g++ : 用語 'g++' は ... 認識されません` のようなエラーが出る場合は、

- PATH のスペルミスがないか（0.6 をもう一度確認）
- ターミナルを開き直したか
- `C:\dev\mingw64\bin\g++.exe` が本当に存在するか（zip の展開場所のミス）

を確認してください。

## 0.8 VS Code の導入

1. https://code.visualstudio.com/ から **User Installer**（既定のダウンロード）を入手して
   インストールする
   - User Installer は管理者権限不要です
2. インストール時「PATH への追加」はチェックを入れたまま（既定のまま）進めてOK

### 拡張機能のインストール

VS Code を起動し、左端の四角いアイコン（拡張機能、`Ctrl+Shift+X`）から
次の3つを検索してインストールします。

| 拡張機能 | ID | 用途 |
|----------|----|------|
| **C/C++** | `ms-vscode.cpptools` | コード補完・エラー表示・デバッグ |
| **CMake Tools** | `ms-vscode.cmake-tools` | CMake プロジェクトのビルド・実行をボタン操作で |
| Japanese Language Pack | `MS-CEINTL.vscode-language-pack-ja` | メニューの日本語化（任意） |

> このチュートリアルのフォルダを VS Code で開くと、右下に
> 「このワークスペースには推奨拡張機能があります」と通知が出ます。
> そこから「インストール」を押してもOKです（`.vscode/extensions.json` に定義してあります）。

## 0.9 日本語の文字化け対策（重要）

C++ のソースコードは **UTF-8** で保存します（VS Code の既定設定のままでOK）。
一方、Windows のターミナルは既定で Shift_JIS（CP932）表示のため、
プログラムが日本語を出力すると**文字化けすることがあります**。

文字化けしたら、ターミナルで次を実行してから再度プログラムを実行してください。

```powershell
chcp 65001
```

これはターミナルの表示を UTF-8 に切り替えるコマンドです（そのターミナルを閉じるまで有効）。
毎回打つのが面倒な場合は、プログラム側の出力を英数字だけにするのも実用的な回避策です。

## 0.10 チェックリスト

- [ ] `g++ --version` でバージョンが表示される
- [ ] `cmake --version` でバージョンが表示される
- [ ] `ninja --version` でバージョンが表示される
- [ ] VS Code に C/C++ と CMake Tools 拡張が入っている

すべて確認できたら、[第1章 はじめてのビルド](01_first_build.md) へ進みましょう。
