#pragma once

#include <utility>
#include <vector>

namespace numlib {

// 平均と標準偏差（母標準偏差）を返す
std::pair<double, double> MeanAndStddev(const std::vector<double>& values);

// 演習1: 最小値と最大値を返す
std::pair<double, double> MinMax(const std::vector<double>& values);

}  // namespace numlib
