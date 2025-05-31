#pragma once

#include <cstddef>


constexpr std::size_t operator"" _KiB(long double value) {
    return static_cast<std::size_t>(value * 1024);
}

constexpr std::size_t operator"" _MiB(long double value) {
    return static_cast<std::size_t>(value * 1024 * 1024);
}

constexpr std::size_t operator"" _GiB(long double value) {
    return static_cast<std::size_t>(value * 1024 * 1024 * 1024);
}

constexpr std::size_t operator"" _TiB(long double value) {
    return static_cast<std::size_t>(value * 1024 * 1024 * 1024 * 1024);
}

constexpr std::size_t operator"" _KB(long double value) {
    return static_cast<std::size_t>(value * 1000);
}

constexpr std::size_t operator"" _MB(long double value) {
    return static_cast<std::size_t>(value * 1000 * 1000);
}

constexpr std::size_t operator"" _GB(long double value) {
    return static_cast<std::size_t>(value * 1000 * 1000 * 1000);
}

constexpr std::size_t operator"" _TB(long double value) {
    return static_cast<std::size_t>(value * 1000 * 1000 * 1000 * 1000 );
}
