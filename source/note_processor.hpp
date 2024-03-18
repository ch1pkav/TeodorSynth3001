#ifndef TEODORSYNTH3001_NOTE_PROCESSOR_HPP
#define TEODORSYNTH3001_NOTE_PROCESSOR_HPP
#include <array>
#include <cmath>
#include "public.sdk/source/vst/vstaudioeffect.h"

using namespace Steinberg;

static inline constexpr auto maxPolyphony = 16;

namespace ts3k1 {
    class NoteProcessor {
        std::array<double, maxPolyphony> masterOscFrequency;
        size_t notesOn = 0;
    public:
        NoteProcessor(): masterOscFrequency() {
            masterOscFrequency.fill(0.);
        }
        void registerNoteOn(double frequency) {
            if (notesOn < maxPolyphony) {
                masterOscFrequency[notesOn] = frequency;
                notesOn++;
            }
        }
        void registerNoteOff(double frequency) {
            for (size_t i = 0; i < maxPolyphony; i++) {
                if (masterOscFrequency[i] == frequency) {
                    masterOscFrequency[i] = 0.;
                    notesOn--;
                    memmove(masterOscFrequency.data()+i, masterOscFrequency.data()+i+1, (maxPolyphony-i-1)*sizeof(double));
                    break;
                }
            }
        }
        [[nodiscard]] auto getMasterOscFrequency() const {
            return masterOscFrequency;
        }
    };
}

#endif //TEODORSYNTH3001_NOTE_PROCESSOR_HPP
