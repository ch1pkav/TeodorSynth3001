//
// Created by vepbxer on 3/17/24.
//

#ifndef TEODORSYNTH3001_COMMON_HPP
#define TEODORSYNTH3001_COMMON_HPP
#include <numbers>

namespace ts3k1 {
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
}

static inline constexpr auto f2PI = std::numbers::pi * 2.;
#endif //TEODORSYNTH3001_COMMON_HPP
