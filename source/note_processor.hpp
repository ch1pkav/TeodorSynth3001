#ifndef TEODORSYNTH3001_NOTE_PROCESSOR_HPP
#define TEODORSYNTH3001_NOTE_PROCESSOR_HPP
#include <array>
#include <cmath>
#include "public.sdk/source/vst/vstaudioeffect.h"

using namespace Steinberg;

static inline constexpr auto maxPolyphony = 16;

namespace ts3k1 {
    template<class Voice>
    using VoiceProcessor = std::array<Voice, maxPolyphony>;
}

#endif //TEODORSYNTH3001_NOTE_PROCESSOR_HPP
