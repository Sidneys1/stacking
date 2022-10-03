#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

template <class T> std::string FormatWithCommas(T value) {
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss << std::fixed << value;
    return ss.str();
}

template <typename E>
concept Enum = std::is_enum<E>::value;

template <typename E>
concept EnumWithEnd = requires(E e) {
    Enum<E>;
    E::END_OF_LIST;
};

template <typename E> E &operator++(E &e) requires EnumWithEnd<E> {
    // using IntType = typename std::underlying_type<E>::type;
    e = static_cast<E>(std::to_underlying(e) + 1);
    if (e == E::END_OF_LIST)
        e = static_cast<E>(0);
    return e;
}

template <typename E> E &operator--(E &e) requires EnumWithEnd<E> {
    // using IntType = typename std::underlying_type<E>::type;
    if (std::to_underlying(e) == 0)
        e = E::END_OF_LIST;
    else
        e = static_cast<E>(std::to_underlying(e) - 1);
    return e;
}
