//------------------------------------------------------------------------
// Copyright(c) 2024 Teodor Synths.
//------------------------------------------------------------------------

#include "TS3K1_processor.h"
#include "TS3K1_cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "common.hpp"
#include <cmath>
#include <iostream>

using namespace Steinberg;

namespace ts3k1 {
//------------------------------------------------------------------------
// CTeodorSynth3001Processor
//------------------------------------------------------------------------
CTeodorSynth3001Processor::CTeodorSynth3001Processor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kCTeodorSynth3001ControllerUID);
}

//------------------------------------------------------------------------
CTeodorSynth3001Processor::~CTeodorSynth3001Processor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                paramQueue -> getPoint (numPoints - 1, sampleOffset, value);
                for (auto & voice: voices)
                switch (paramQueue->getParameterId ())
                {
                    case kOsc1:
                        voice.Osc1 = static_cast<double>(value);
                        break;
                    case kOsc2:
                        voice.Osc2 = static_cast<double>(value);
                        break;
                    case kDecay:
                        voice.Decay = static_cast<double>(value);
                        break;
				}
			}
		}
	}
	
	//--- Here you have to implement your processing
    auto * events = data.inputEvents;
    if (events)
    {
        for (int32 i = 0; i < events->getEventCount (); i++)
        {
            Vst::Event event {};
            if (events->getEvent (i, event) == kResultOk)
            {
                    if (event.type == Vst::Event::kNoteOnEvent) {
                        for (auto& voice: voices) {
                            if (voice.masterVolume <= 0.00001) {
                                voice.masterOscFrequency = 440. * std::pow(2., (event.noteOn.pitch - 69.) / 12.);
                                voice.masterOscDeltaAngle = voice.masterOscFrequency / data.processContext->sampleRate * f2PI;
                                voice.masterVolume = 0.3;
                                voice.Osc1Phase = 0.;
                                voice.Osc2Phase = 0.;
                                voice.pitch = event.noteOn.pitch;
                                break;
                            }
                        }
                    }
                    else if (event.type == Vst::Event::kNoteOffEvent) {
                        for (auto& voice: voices) if (event.noteOff.pitch == voice.pitch) {
                            voice.masterVolume = 0;
                        }
                    }
            }
        }
    }

    if (data.numInputs == 0 || data.numOutputs == 0) {
        return kResultOk;
    }


    if (data.symbolicSampleSize == Vst::kSample32)
    {
        if (data.numSamples > 0)
        {
            float out = 0;
            for (int32 sample = 0; sample < data.numSamples; sample++)
            {
                for (auto& voice: voices) {
                    float osc1 = voice.Osc1 * sgn(std::sin(voice.Osc1Phase));
                    float osc2 = voice.Osc2 * std::sin(voice.Osc2Phase);
                    voice.Osc1Phase += voice.masterOscDeltaAngle * 2;
                    voice.Osc2Phase += voice.masterOscDeltaAngle;
                    out += (osc1 + osc2) * voice.masterVolume;
                    if (sample % 1000 == 0)
                        voice.masterVolume *= voice.Decay;
                }
                for (int32 channel = 0; channel < data.outputs[0].numChannels; channel++)
                {
                    data.outputs[0].channelBuffers32[channel][sample] = out;
                }

            }
        }
    }


	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
//	if (symbolicSampleSize == Vst::kSample64)
//		return kResultTrue;

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);

//    double fval;
//    if (!streamer.readDouble(fval)) return kResultFalse;
//    Osc1 = fval;
//    if (!streamer.readDouble(fval)) return kResultFalse;
//    Osc2 = fval;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);
//    streamer.writeDouble(Osc1);
//    streamer.writeDouble(Osc2);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace ts3k1
