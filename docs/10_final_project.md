# 第10章 総合演習 — ばね・質量・ダンパ系のシミュレーション

最終章では、これまで学んだすべてを使って**1自由度振動系（ばね・質量・ダンパ系）の
数値シミュレータ**を作ります。機械力学で学ぶ（学んだ/これから学ぶ）あの系です。

```
        k          ┌─────┐
  ┤├─/\/\/\──┤     │  m  │
  ┤├───[c]───┤     └─────┘
   壁   ダンパ        質量  → x
```

運動方程式:

```
m·ẍ + c·ẋ + k·x = 0
（m: 質量 [kg], c: 減衰係数 [N·s/m], k: ばね定数 [N/m]）
```

> **この章も独立プロジェクトです。** `examples/ch10_project` を VS Code で
> 単独で開いてください。まず自力で設計・実装に挑戦し、
> 詰まったらサンプルを読む、という進め方を推奨します。

完成版コード: [`examples/ch10_project/`](../examples/ch10_project/)

## 10.1 要求仕様

作るものを実務風に「仕様」として書きます。

1. m, c, k, 初期変位 x₀, 初期速度 v₀, 時間刻み dt, 終了時刻 t_end を与えると、
   x(t) と v(t) の時間履歴を計算する
2. 数値積分法は**セミインプリシット・オイラー法**と **RK4（4次ルンゲクッタ法）** の
   2種類を実装し、切り替えられる
3. 結果を CSV ファイルに出力する（Excel や Python でグラフ化して確認）
4. 積分器はライブラリとして分離し、**CTest でテストを付ける**
5. 減衰がないとき（c=0）はエネルギーが保存に近いこと、
   理論解と比較して誤差が小さいことをテストで確認する

## 10.2 数値解法の整理

2階の運動方程式は、状態 (x, v) の1階連立ODEに直します。

```
ẋ = v
v̇ = -(c·v + k·x) / m
```

**セミインプリシット・オイラー法**（v を先に更新して、その v で x を更新する）:

```
v ← v + a(x, v)·dt
x ← x + v·dt
```

単純なオイラー法（x, v を同時に更新）よりエネルギーの発散が起きにくく、
物理シミュレーションの定番です。

**RK4** は精度が高い定番手法です（詳細はサンプルコード参照）。
数値解析の授業で式を見たことがあるはずです。

**理論解**（テストに使う）: 不足減衰 (ζ < 1) のとき

```
x(t) = e^(-ζωt) · ( x₀·cos(ω_d t) + (v₀ + ζω x₀)/ω_d · sin(ω_d t) )
ω = √(k/m),  ζ = c / (2√(mk)),  ω_d = ω√(1-ζ²)
```

## 10.3 推奨プロジェクト構成

```
ch10_project/
├── CMakeLists.txt
├── include/
│   └── sim/
│       ├── model.hpp        ← State 構造体・SpringMassDamper クラス
│       └── integrator.hpp   ← 積分器（euler_step / rk4_step / simulate）
├── src/
│   ├── model.cpp
│   └── integrator.cpp
├── apps/
│   └── main.cpp             ← パラメータ設定・シミュレーション実行・CSV出力
└── tests/
    └── test_integrator.cpp  ← 理論解との比較テスト
```

## 10.4 設計のヒント

どの章の道具を使うかの対応表です。詰まったら該当章を読み返してください。

| 設計判断 | 使う道具 | 章 |
|----------|---------|----|
| 状態 (t, x, v) の表現 | `struct State { double t, x, v; };` | 4 |
| 系のパラメータの保持と加速度計算 | `SpringMassDamper` クラス（`accel(x, v)` メンバ関数） | 4 |
| 不正なパラメータ (m ≤ 0) の拒否 | コンストラクタで `throw` | 4, 7 |
| 積分法の切り替え | 積分1ステップを関数に切り出し、`std::function` かenumで選択 | 3, 8 |
| 時間履歴の格納 | `std::vector<State>` を返す `simulate()` 関数 | 2, 3 |
| CSV出力 | `std::ofstream` | 5 |
| ライブラリ分離 | `add_library(sim STATIC ...)` | 8 |
| テスト | `enable_testing()` + `add_test` | 9 |
| 最大振幅の検出（減衰の確認） | `std::max_element` + ラムダ | 5 |

## 10.5 進め方（推奨ステップ）

1. **Step 1:** `SpringMassDamper` クラスと `State` を作り、
   オイラー法だけで main.cpp に直書きのシミュレーションを動かす。CSVに出す
2. **Step 2:** Excel か Python で x(t) をプロットして「減衰振動の形」になっているか
   目視確認する。dt を大きくすると解が発散することも観察する
3. **Step 3:** 積分処理を `include/sim/` + `src/` のライブラリに分離する
4. **Step 4:** RK4 を追加し、同じ dt でオイラー法と精度を比較する
5. **Step 5:** 理論解との比較テストを書き、`ctest` で回す

「まず雑に動かす → 動いたら整理する → 整理できたらテストで守る」という順番は、
実務のプロトタイピングそのものです。

## 10.6 CSV出力とグラフ化

```cpp
std::ofstream out("result.csv");
out << "t,x,v\n";
for (const State& s : history) {
    out << s.t << "," << s.x << "," << s.v << "\n";
}
```

グラフ化の例（Python がある場合）:

```python
import pandas as pd
import matplotlib.pyplot as plt
df = pd.read_csv("result.csv")
df.plot(x="t", y="x")
plt.xlabel("time [s]"); plt.ylabel("x [m]")
plt.show()
```

Excel でも「result.csv を開く → 散布図（直線）」で確認できます。

## 10.7 発展課題

余力があれば挑戦してください。

1. **強制振動:** 外力 F(t) = F₀·sin(ωt) を加え、周波数 ω を掃引して
   共振曲線（ω vs 最大振幅）をCSV出力する
2. **パラメータをコマンドライン引数から取る:** `main(int argc, char* argv[])` で
   `sim.exe 1.0 0.5 100.0` のように m, c, k を渡せるようにする
   （`std::stod` で変換。引数不足のときの処理も忘れずに）
3. **2自由度系:** 質量2つ・ばね3つの系に拡張する（State を `std::vector<double>` 化
   するか、質点ごとの State を持つか、設計から考えてみましょう）
4. **fmt導入:** 第9章の FetchContent で fmt を入れ、実行中の進捗表示を整える

## 10.8 修了

ここまで完走したら、あなたは

- モダンC++（string / vector / クラス / STL / スマートポインタ / テンプレートの読み書き）
- CMake による実務スタイルのプロジェクト構成（ライブラリ分割・外部依存・テスト）
- VS Code での開発サイクル（編集 → ビルド → 実行 → デバッグ → テスト）

を一通り身につけたことになります。

### 次に学ぶと良いもの

- **Eigen** … 行列・線形代数ライブラリ。有限要素法や制御工学の計算に（FetchContentで入ります）
- **GoogleTest / Catch2** … 本格的なテストフレームワーク
- **Git / GitHub** … コードのバージョン管理。研究室・チーム開発の必須スキル
- **C++20 の ranges / concepts** … STLがさらに書きやすくなる新機能
- 書籍・サイト: [cppreference.com](https://ja.cppreference.com/)（リファレンスの決定版）、
  [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)（プロの作法集）

お疲れさまでした！
