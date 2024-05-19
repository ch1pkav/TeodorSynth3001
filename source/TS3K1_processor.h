//------------------------------------------------------------------------
// Copyright(c) 2024 Teodor Synths.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "params.h"
#include "baseProcessor.h"
#include <numbers>

namespace ts3k1 {
using namespace Steinberg;
//------------------------------------------------------------------------
//  CTeodorSynth3001Processor
//------------------------------------------------------------------------
class CTeodorSynth3001Processor : public Steinberg::Vst::mda::BaseProcessor
{
public:
    using Base = Steinberg::Vst::mda::BaseProcessor;
	CTeodorSynth3001Processor ();
	~CTeodorSynth3001Processor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new CTeodorSynth3001Processor; 
	}

    // Unique ID
	int32 getVst2UniqueId () const SMTG_OVERRIDE { return 'TS3K'; }

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
//	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
//	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
//	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

	void doProcessing (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;

//	bool hasProgram () const SMTG_OVERRIDE { return true; }
//	Steinberg::uint32 getCurrentProgram () const SMTG_OVERRIDE { return currentProgram; }
//	Steinberg::uint32 getNumPrograms () const SMTG_OVERRIDE { return kNumPrograms; }
//	void setCurrentProgram (Steinberg::uint32 val) SMTG_OVERRIDE;
//	void setCurrentProgramNormalized (ParamValue val) SMTG_OVERRIDE;
//------------------------------------------------------------------------
protected:
    struct VOICE {
        float delta;
        float Osc1 = defaultOsc1;
        float Osc2 = defaultOsc2;
        float Osc1Phase = 0;
        float Osc2Phase = 0;
        float env;  //envelope
        float dec;

        int32 note; //remember what note triggered this
        int32 noteID;
    };

    int32 poly;
    static constexpr int32 kNumVoices = 32;
    static constexpr int32 kEventBufferSize = 64;
    using SynthDataT = Steinberg::Vst::mda::SynthData<VOICE, kEventBufferSize, kNumVoices>;
    SynthDataT synthData;

	void setParameter (Steinberg::Vst::ParamID index, Steinberg::Vst::ParamValue newValue, int32 sampleOffset) SMTG_OVERRIDE;
	void preProcess () SMTG_OVERRIDE;
	void processEvent (const Vst::Event& event) SMTG_OVERRIDE;
	void noteEvent (const Vst::Event& event);
};

//------------------------------------------------------------------------
} // namespace ts3k1
