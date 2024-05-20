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
#include <algorithm>
#include <atomic>
#include <execution>

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

    if (data.inputParameterChanges) {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
        for (int32 index = 0; index < numParamsChanged; index++) {
            if (auto *paramQueue = data.inputParameterChanges->getParameterData(index)) {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount();
                paramQueue->getPoint(numPoints - 1, sampleOffset, value);
                switch (paramQueue->getParameterId()) {
                    case kOsc1:
                        Osc1 = static_cast<double>(value);
                        break;
                    case kOsc2:
                        Osc2 = static_cast<double>(value);
                        break;
                    case kNoise:
                        Noise = static_cast<double>(value);
                        break;
                    case kWav1:
                        Wav1 = static_cast<double>(value);
                        break;
                    case kWav2:
                        Wav2 = static_cast<double>(value);
                        break;
                    case kAttack:
                        Attack = static_cast<double>(value);
                        break;
                    case kDecay:
                        Decay = static_cast<double>(value);
                        break;
                    case kLPCutoff:
                        LPCutoff = static_cast<double>(value)*100;
                        break;
                    case kLPOn:
                        LPOn = static_cast<double>(value);
                        break;
                    case kLPEnv:
                        LPEnv = static_cast<double>(value);
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
                            if (voice.volume <= 0.00001) {
                                voice.freq = 440. * std::pow(2., (event.noteOn.pitch - 69.) / 12.);
                                voice.deltaAngle = voice.freq / data.processContext->sampleRate * f2PI;
                                voice.lowPassFilter.setSampleRate(data.processContext->sampleRate);
                                voice.volume = 0.0001;
                                voice.lowPassFilter.setCutoff(LPCutoff);
                                voice.Osc1Phase = 0.;
                                voice.Osc2Phase = 0.;
                                voice.noise = Noise;
                                voice.pitch = event.noteOn.pitch;
                                voice.envelopeState = EnvelopeState::Attack;
                                if (Wav1 < 0.25) {
                                    voice.waveform1 = sine_wave;
                                } else if (Wav1 < 0.5) {
                                    voice.waveform1 = square_wave;
                                } else if (Wav1 < 0.75) {
                                    voice.waveform1 = saw_wave;
                                } else {
                                    voice.waveform1 = triangle_wave;
                                }

                                if (Wav2 < 0.25) {
                                    voice.waveform2 = sine_wave;
                                } else if (Wav2 < 0.5) {
                                    voice.waveform2 = square_wave;
                                } else if (Wav2 < 0.75) {
                                    voice.waveform2 = saw_wave;
                                } else {
                                    voice.waveform2 = triangle_wave;
                                }
                                break;
                            }
                        }
                    }
                    else if (event.type == Vst::Event::kNoteOffEvent) {
                        for (auto& voice: voices) if (event.noteOff.pitch == voice.pitch) {
                            voice.volume = 0;
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
            std::atomic<double> out = 0;
            for (int32 sample = 0; sample < data.numSamples; sample++)
            {
                std::for_each(std::execution::par_unseq, voices.begin(), voices.end(), [&](auto & voice) {
                    if (voice.volume <= 0.00001) {
                        return;
                    }
                    double osc1 = Osc1 * voice.waveform1(voice.Osc1Phase);
                    double osc2 = Osc2 * voice.waveform2(voice.Osc2Phase);
                    double noise_ = voice.noise * noise(voice.Osc1Phase);
                    voice.Osc1Phase += voice.deltaAngle * 2;
                    voice.Osc2Phase += voice.deltaAngle;
                    if (LPOn > 0.5) {
                        out += voice.lowPassFilter.process(osc1 + osc2 + noise_) * voice.volume;
                    } else {
                        out += (osc1 + osc2 + noise_) * voice.volume;
                    }
                    if (sample % 8000 == 0) {
                        if (voice.envelopeState == EnvelopeState::Attack) {
                            voice.volume /= Attack;
                            if (voice.volume >= 0.2) {
                                voice.envelopeState = EnvelopeState::Decay;
                            }
                        } else if (voice.envelopeState == EnvelopeState::Decay) {
                            voice.volume *= Decay;
                        }
                    }
                    if (LPEnv > 0.5) {
                        voice.lowPassFilter.setCutoff(LPCutoff * voice.volume * 2);
                    }
                });
                for (int32 channel = 0; channel < data.outputs[0].numChannels; channel++)
                {
                    data.outputs[0].channelBuffers32[channel][sample] = out / voices.size();
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

    double fval;
    if (!streamer.readDouble(fval)) return kResultFalse;
    Osc1 = fval;
    if (!streamer.readDouble(fval)) return kResultFalse;
    Osc2 = fval;
    if (!streamer.readDouble(fval)) return kResultFalse;
    Attack = fval;
    if (!streamer.readDouble(fval)) return kResultFalse;
    Decay = fval;
    if (!streamer.readDouble(fval)) return kResultFalse;
    LPCutoff = fval;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);
    streamer.writeDouble(Osc1);
    streamer.writeDouble(Osc2);
    streamer.writeDouble(Attack);
    streamer.writeDouble(Decay);
    streamer.writeDouble(LPCutoff);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace ts3k1
