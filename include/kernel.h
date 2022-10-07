#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <numbers>
#include <type_traits>

template <typename T>
concept FloatingPoint = std::is_floating_point<T>::value;

template <std::size_t size, FloatingPoint precision_t = double> class Kernel {
  public:
    using KType = std::array<std::array<precision_t, size>, size>;

  public:
    static constexpr std::size_t Size = size;
    static KType Values;
};

void foo();
