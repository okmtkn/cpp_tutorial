#pragma once

#include <utility>
#include <vector>

namespace numlib {

// 平均と標準偏差（母標準偏差）を返す
std::pair<double, double> mean_stddev(const std::vector<double>& v);

// 演習1: 最小値と最大値を返す
std::pair<double, double> min_max(const std::vector<double>& v);

}  // namespace numlib
