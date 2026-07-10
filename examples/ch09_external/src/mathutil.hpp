#pragma once

#include <optional>
#include <vector>

// 台形則で sin(x) を [a, b] で積分する
double trapezoid_sin(double a, double b, int n = 1000);

// 平均（空の vector を渡してはいけない）
double mean(const std::vector<double>& v);

// 平均の安全版: 空なら nullopt
std::optional<double> mean_safe(const std::vector<double>& v);

// 演習1: 二乗平均平方根 (RMS)
double rms(const std::vector<double>& v);
