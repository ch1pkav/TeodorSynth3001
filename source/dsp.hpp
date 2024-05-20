//
// Created by vepbxer on 3/17/24.
//

#ifndef TEODORSYNTH3001_DSP_HPP
#define TEODORSYNTH3001_DSP_HPP

#include <functional>
#include "common.hpp"

namespace ts3k1 {
    inline auto square_wave = [](double phase) {
        return (double) sgn(std::sin(phase));
    };

    inline auto saw_wave = [](double phase) {
        phase = std::fmod(phase, 1.);
        return 2. * phase - 1.;
    };

    inline auto triangle_wave = [](double phase) {
        phase = std::fmod(phase, 1.);
        return 2. * std::abs(2. * phase - 1.) - 1.;
    };

    inline auto noise = [](double) {
        return 2. * (std::rand() / static_cast<double>(RAND_MAX)) - 1.;
    };

    inline auto sine_wave = [](double phase) {
        return std::sin(phase);
    };

    using waveform_t = std::function<double(double)>;

}// namespace ts3k1
#endif //TEODORSYNTH3001_DSP_HPP
