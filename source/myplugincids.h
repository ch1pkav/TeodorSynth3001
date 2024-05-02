//------------------------------------------------------------------------
// Copyright(c) 2024 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace MyCompanyName {
//------------------------------------------------------------------------
static const Steinberg::FUID kTeodorSynth3001ProcessorUID (0x95CD5696, 0x869050EE, 0x8F3A8D61, 0x955BEDF4);
static const Steinberg::FUID kTeodorSynth3001ControllerUID (0x45EAC8E9, 0x289F5751, 0x8E9FADD2, 0xC9DB7141);

#define TeodorSynth3001VST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace MyCompanyName
