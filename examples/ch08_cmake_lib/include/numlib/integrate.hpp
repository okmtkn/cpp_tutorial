#pragma once  // 二重インクルード防止（#ifndef ガードの現代的な書き方）

#include <functional>

namespace numlib {

// 台形則による数値積分: f を [a, b] で n 分割して積分する
// std::function<double(double)> は「double を受けて double を返す関数」を
// 受け取れる型。普通の関数もラムダ式も渡せる。
double trapezoid(const std::function<double(double)>& f,
                 double a, double b, int n = 1000);

}  // namespace numlib
