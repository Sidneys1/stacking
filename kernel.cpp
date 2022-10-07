#include "kernel.h"

template <std::size_t size, typename precision_t> static std::array<std::array<precision_t, size>, size> gen_kernel() {
    std::array<std::array<precision_t, size>, size> ret = {};

    // initialising standard deviation to 1.0
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;

    // sum is for normalization
    double sum = 0.0;

    // generating 5x5 kernel
    long long half = size / 2;
    for (long long x = -half; x <= half; ++x) {
        for (long long y = -half; y <= half; ++y) {
            r = std::sqrt(x * x + y * y);
            ret[x + half][y + half] = (exp(-(r * r) / s)) / (std::numbers::pi * s);
            sum = sum + ret[x + half][y + half];
        }
    }

    if (sum == 0.0f)
        return ret;

    // normalising the Kernel
    for (std::size_t i = 0; i < size; ++i)
        for (std::size_t j = 0; j < size; ++j)
            ret[i][j] = ret[i][j] / sum;

    return ret;
}

template <std::size_t size, FloatingPoint precision_t>
typename Kernel<size, precision_t>::KType Kernel<size, precision_t>::Values = gen_kernel<size, precision_t>();
