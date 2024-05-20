//------------------------------------------------------------------------
// Copyright(c) 2024 Teodor Synths.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "params.h"
#include "note_processor.hpp"
#include "common.hpp"
#include <numbers>
#include "dsp.hpp"

namespace ts3k1 {

//------------------------------------------------------------------------
//  CTeodorSynth3001Processor
//------------------------------------------------------------------------
class CTeodorSynth3001Processor : public Steinberg::Vst::AudioEffect
{
public:
	CTeodorSynth3001Processor ();
	~CTeodorSynth3001Processor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new CTeodorSynth3001Processor; 
	}

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:
    enum class EnvelopeState {
        Attack,
        Decay,
    };

    struct Voice {
        double Osc1Phase = 0.;
        double Osc2Phase = 0.;
        double freq = 0.;
        double volume = 0.;
        double deltaAngle = 0.;
        double noise = 0.;
        int16 pitch;
        EnvelopeState envelopeState = EnvelopeState::Attack;
        LowPassFilter lowPassFilter{44100, defaultLPCutoff};
        waveform_t waveform1 = sine_wave;
        waveform_t waveform2 = sine_wave;
    };

    double Osc1 = defaultOsc1;
    double Osc2 = defaultOsc2;
    double Noise = defaultNoise;
    double Wav1 = defaultWav1;
    double Wav2 = defaultWav2;
    double Attack = defaultAttack;
    double Decay = defaultDecay;
    double LPCutoff = defaultLPCutoff*10000;
    double LPOn = defaultLPOn;
    double LPEnv = defaultLPEnv;

    VoiceProcessor<Voice> voices;
};

//------------------------------------------------------------------------
} // namespace ts3k1
