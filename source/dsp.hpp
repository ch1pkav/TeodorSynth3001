//
// Created by vepbxer on 3/17/24.
//

#ifndef TEODORSYNTH3001_DSP_HPP
#define TEODORSYNTH3001_DSP_HPP

#include <functional>
#include "common.hpp"

namespace ts3k1 {
    using waveform_t = std::function<double(double)>;

    inline waveform_t square_wave = [](double phase) {
        return (double) sgn(std::sin(phase));
    };

    inline waveform_t saw_wave = [](double phase) {
        phase = std::fmod(phase, 1.);
        return 2. * phase - 1.;
    };

    inline waveform_t triangle_wave = [](double phase) {
        phase = std::fmod(phase, 1.);
        return 2. * std::abs(2. * phase - 1.) - 1.;
    };

    inline waveform_t noise = [](double) {
        return 2. * (std::rand() / static_cast<double>(RAND_MAX)) - 1.;
    };

    inline waveform_t sine_wave = [](double phase) {
        return std::sin(phase);
    };


}// namespace ts3k1
#endif //TEODORSYNTH3001_DSP_HPP
