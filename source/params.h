#pragma once

enum
{
    kOsc1 = 100,
    kOsc2,
    kAttack = 200,
    kDecay,
    kLPCutoff = 300
};

static constexpr auto defaultOsc1 = 0.5;
static constexpr auto defaultOsc2 = 0.5;
static constexpr auto defaultDecay = 0.8;
static constexpr auto defaultAttack = 0.1;
static constexpr auto defaultLPCutoff = 0.03;
