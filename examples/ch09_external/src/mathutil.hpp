#pragma once

#include <optional>
#include <vector>

// 台形則で sin(x) を [lower_limit, upper_limit] で積分する
double IntegrateSinTrapezoid(double lower_limit, double upper_limit,
                             int num_intervals = 1000);

// 平均（空の vector を渡してはいけない）
double Mean(const std::vector<double>& values);

// 平均の安全版: 空なら nullopt
std::optional<double> SafeMean(const std::vector<double>& values);

// 演習1: 二乗平均平方根 (RMS: root mean square)
double RootMeanSquare(const std::vector<double>& values);
