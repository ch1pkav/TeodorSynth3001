//
// Created by vepbxer on 3/17/24.
//

#ifndef TEODORSYNTH3001_COMMON_HPP
#define TEODORSYNTH3001_COMMON_HPP


namespace ts3k1 {
    static inline constexpr auto f2PI = std::numbers::pi * 2.;
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    class LowPassFilter {
    public:
        explicit LowPassFilter(float sampleRate, float cutoff) : sampleRate(sampleRate), cutoff(cutoff) {
            auto RC = 1. / (cutoff * 2 * std::numbers::pi);
            auto dt = 1. / sampleRate;
            alpha = dt / (RC + dt);
            y = 0;
        }

        float process(float x) {
            y = alpha * x + (1 - alpha) * y;
            return y;
        }

        float getCutoff() const {
            return cutoff;
        }

        void setCutoff(float cutoff_) {
            LowPassFilter::cutoff = cutoff_;
            auto RC = 1. / (cutoff * 2 * std::numbers::pi);
            auto dt = 1. / sampleRate;
            alpha = dt / (RC + dt);
        }

        float getSampleRate() const {
            return sampleRate;
        }

        void setSampleRate(float sampleRate_) {
            LowPassFilter::sampleRate = sampleRate_;
            auto RC = 1. / (cutoff * 2 * std::numbers::pi);
            auto dt = 1. / sampleRate;
            alpha = dt / (RC + dt);
        }

    private:
        float sampleRate;
        float cutoff;
        float alpha;
        float y;
    };
}

#endif //TEODORSYNTH3001_COMMON_HPP
