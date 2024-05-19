//
// Created by vepbxer on 3/17/24.
//

#ifndef TEODORSYNTH3001_DSP_HPP
#define TEODORSYNTH3001_DSP_HPP
#include "TS3K1_processor.h"

namespace ts3k1 {
    float lowPassFilter(float input, float cutoff, float resonance) {
        static float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
        float c = 1.0f / tanf(M_PI * cutoff / 44100.0f);
        float a1 = 1.0f / (1.0f + resonance * c + c * c);
        float a2 = 2 * a1;
        float a3 = a1;
        float b1 = 2.0f * (1.0f - c * c) * a1;
        float b2 = (1.0f - resonance * c + c * c) * a1;
        float output = a1 * input + a2 * x1 + a3 * x2 - b1 * y1 - b2 * y2;
        x2 = x1;
        x1 = input;
        y2 = y1;
        y1 = output;
        return output;
    }

}// namespace ts3k1
#endif //TEODORSYNTH3001_DSP_HPP
