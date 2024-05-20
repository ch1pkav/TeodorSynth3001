#pragma once

enum
{
    kOsc1 = 100,
    kOsc2,
    kNoise,
    kWav1,
    kWav2,
    kAttack = 200,
    kDecay,
    kLPCutoff = 300,
    kLPOn,
    kLPEnv,
    kMasterVolume = 400,
};

static constexpr auto defaultOsc1 = 0.5;
static constexpr auto defaultOsc2 = 0.5;
static constexpr auto defaultNoise = 0.1;
static constexpr auto defaultWav1 = 0.5;
static constexpr auto defaultWav2 = 0.5;
static constexpr auto defaultDecay = 0.8;
static constexpr auto defaultAttack = 0.1;
static constexpr auto defaultLPCutoff = 0.03;
static constexpr auto defaultLPOn = 0.;
static constexpr auto defaultLPEnv = 0.;
static constexpr auto defaultMasterVolume = 0.3;
