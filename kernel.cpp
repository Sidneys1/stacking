#include <fmt/format.h>

#include "kernel.h"

template <std::size_t size, FloatingPoint precision_t>
consteval std::array<std::array<precision_t, size>, size> gen_kernel() {
    static_assert(size > 0);
    static_assert(size % 2 == 1);
    std::array<std::array<precision_t, size>, size> ret = {};

    // initialising standard deviation to 1.0
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;

    // sum is for normalization
    double sum = 0.0;

    // generating kernel
    long long half = size / 2;
    for (long long x = -half; x <= half; ++x) {
        for (long long y = -half; y <= half; ++y) {
            // r = std::sqrt(x * x + y * y);
            r = gcem::sqrt(x * x + y * y);
            ret[x + half][y + half] = (gcem::exp(-(r * r) / s)) / (std::numbers::pi * s);
            sum = sum + ret[x + half][y + half];
        }
    }

    // Bail if we somehow encounter a divide by zero
    if (sum == 0.0f)
        return ret;

    // normalising the Kernel
    for (std::size_t i = 0; i < size; ++i)
        for (std::size_t j = 0; j < size; ++j)
            ret[i][j] = ret[i][j] / sum;

    return ret;
}

void dump_kernel() {
    auto kernel = Kernel<7>::Values;
    double sum = 0;
    for (auto &row : kernel) {
        for (auto column : row) {
            fmt::print("{:<6.2} ", column);
            sum += column;
        }
        fmt::print("\n");
    }
    fmt::print("Sum: {}", sum);
}
