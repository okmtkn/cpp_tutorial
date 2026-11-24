#pragma once  // 二重インクルード防止（#ifndef ガードの現代的な書き方）

#include <functional>

namespace numlib {

// 台形則による数値積分: 被積分関数 integrand を
// [lower_limit, upper_limit] で num_intervals 分割して積分する
// std::function<double(double)> は「double を受けて double を返す関数」を
// 受け取れる型。普通の関数もラムダ式も渡せる。
double IntegrateTrapezoid(const std::function<double(double)>& integrand,
                          double lower_limit, double upper_limit,
                          int num_intervals = 1000);

}  // namespace numlib
