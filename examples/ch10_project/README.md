# ばね・質量・ダンパ系シミュレータ（第10章 総合演習の解答例）

課題の説明は [`../../docs/10_final_project.md`](../../docs/10_final_project.md) を参照。

## 使い方

このフォルダを VS Code で開き、キットを選んでビルド（F7）。

- `sim_app` … シミュレーション実行。`result.csv` に変位 x(t)・速度 v(t) を出力
- `test_integrator` … 理論解との比較テスト（`ctest --test-dir build --output-on-failure`）

## 構成

```
include/sim/model.hpp       State 構造体・SpringMassDamper クラス（運動方程式と理論解）
include/sim/integrator.hpp  セミインプリシット・オイラー法 / RK4 / Simulate()
src/                        実装
apps/main.cpp               パラメータ設定・実行・CSV出力・精度比較
tests/test_integrator.cpp   CTest 用テスト
```
