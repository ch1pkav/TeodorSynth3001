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
	tresult result = Base::initialize (context);
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
	return Base::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return Base::setActive (state);
}

//------------------------------------------------------------------------
//tresult PLUGIN_API CTeodorSynth3001Processor::process (Vst::ProcessData& data)
//{
//	//--- First : Read inputs parameter changes-----------
//
//    if (data.inputParameterChanges)
//    {
//        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
//        for (int32 index = 0; index < numParamsChanged; index++)
//        {
//            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
//            {
//                Vst::ParamValue value;
//                int32 sampleOffset;
//                int32 numPoints = paramQueue->getPointCount ();
//                paramQueue -> getPoint (numPoints - 1, sampleOffset, value);
//                switch (paramQueue->getParameterId ())
//                {
//                    case kOsc1:
//                        Osc1 = static_cast<double>(value);
//                        break;
//                    case kOsc2:
//                        Osc2 = static_cast<double>(value);
//                        break;
//                    case kDecay:
//                        Decay = static_cast<double>(value);
//                        break;
//				}
//			}
//		}
//	}
//
//	//--- Here you have to implement your processing
//    auto * events = data.inputEvents;
//    if (events)
//    {
//        for (int32 i = 0; i < events->getEventCount (); i++)
//        {
//            Vst::Event event {};
//            if (events->getEvent (i, event) == kResultOk)
//            {
//                // TODO: make more sense of this code
//                switch (event.type) {
//                    case Vst::Event::kNoteOnEvent:
//                        masterOscFrequency = 440. * std::pow(2., (event.noteOn.pitch - 69.) / 12.);
//                        masterOscDeltaAngle = masterOscFrequency / data.processContext->sampleRate * f2PI;
//                        masterVolume = 0.3;
//                        Osc1Phase = 0.;
//                        Osc2Phase = 0.;
//                        break;
//                    case Vst::Event::kNoteOffEvent:
//                        masterVolume = 0.;
//                        break;
//                }
//            }
//        }
//    }
//
//    if (data.numInputs == 0 || data.numOutputs == 0) {
//        return kResultOk;
//    }
//
//
//    if (data.symbolicSampleSize == Vst::kSample32)
//    {
//        if (data.numSamples > 0)
//        {
//            for (int32 sample = 0; sample < data.numSamples; sample++)
//            {
//                float osc1 = Osc1 * sgn(std::sin(Osc1Phase));
//                float osc2 = Osc2 * std::sin(Osc2Phase);
//                Osc1Phase += masterOscDeltaAngle * 2;
//                Osc2Phase += masterOscDeltaAngle;
//                float out = (osc1 + osc2) * masterVolume;
//                if (sample % 1000 == 0)
//                    masterVolume *= Decay;
//                for (int32 channel = 0; channel < data.outputs[0].numChannels; channel++)
//                {
//                    data.outputs[0].channelBuffers32[channel][sample] = out;
//                }
//
//            }
//        }
//    }
//
//
//	return kResultOk;
//}
//
//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return Base::setupProcessing (newSetup);
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
//tresult PLUGIN_API CTeodorSynth3001Processor::setState (IBStream* state)
//{
//	// called when we load a preset, the model has to be reloaded
//	IBStreamer streamer (state, kLittleEndian);
//
//    double fval;
//    if (!streamer.readDouble(fval)) return kResultFalse;
//    Osc1 = fval;
//    if (!streamer.readDouble(fval)) return kResultFalse;
//    Osc2 = fval;
//
//	return kResultOk;
//}

//------------------------------------------------------------------------
//tresult PLUGIN_API CTeodorSynth3001Processor::getState (IBStream* state)
//{
//	// here we need to save the model
//	IBStreamer streamer (state, kLittleEndian);
//    streamer.writeDouble(Osc1);
//    streamer.writeDouble(Osc2);
//
//	return kResultOk;
//}


//-----------------------------------------------------------------------------
void CTeodorSynth3001Processor::preProcess ()
{
    synthData.clearEvents ();
}

//-----------------------------------------------------------------------------
void CTeodorSynth3001Processor::processEvent (const Vst::Event& e)
{
    synthData.processEvent (e);
}

//-----------------------------------------------------------------------------
void CTeodorSynth3001Processor::noteEvent(const Vst::Event &event) {
        float l=99.0f;
        int32  v, vl=0;

        if (event.type == Vst::Event::kNoteOnEvent) {
            auto &noteOn = event.noteOn;
            auto note = noteOn.pitch;
//            float velocity = noteOn.velocity * 127;
            if (synthData.activevoices < poly) //add a note
            {
                vl = synthData.activevoices;
                synthData.activevoices++;
            } else //steal a note
            {
                for (v = 0; v < poly; v++)  //find quietest voice
                {
                    if (synthData.voice[v].env < l) {
                        l = synthData.voice[v].env;
                        vl = v;
                    }
                }
            }

            float masterOscFrequency = 440. * std::pow(2., (event.noteOn.pitch - 69.) / 12.);
            synthData.voice[vl].delta = masterOscFrequency / 44100 * f2PI;
            synthData.voice[vl].Osc1Phase = 0.;
            synthData.voice[vl].Osc2Phase = 0.;
            synthData.voice[vl].note = note;
            synthData.voice[vl].noteID = noteOn.noteId;
            synthData.voice[vl].env = 0.7;
            synthData.voice[vl].Osc1 = 1;
            synthData.voice[vl].Osc2 = 1;

        }
        else //note off
        {
            auto& noteOff = event.noteOff;
            auto note = noteOff.pitch;

            for(v=0; v<synthData.numVoices; v++) if (synthData.voice[v].noteID==noteOff.noteId) //any voices playing that note?
            {
                synthData.voice[v].env = 0;//(float)exp (-iFs * exp (6.0 + 0.01 * (double)note - 5.0 * params[1]));
            }
        }
}


void CTeodorSynth3001Processor::doProcessing (Steinberg::Vst::ProcessData& data) {
    int32 sampleFrames = data.numSamples;

    float* out0 = data.outputs[0].channelBuffers32[0];
    float* out1 = data.outputs[0].channelBuffers32[1];

    int32 frame=0, frames, v;
    float l, r;
    int32 i;

    synthData.eventPos = 0;
    if (synthData.activevoices > 0 || synthData.hasEvents ())
    {
        while (frame<sampleFrames)
        {
            frames = synthData.events[synthData.eventPos].sampleOffset;
            if (frames>sampleFrames) frames = sampleFrames;
            frames -= frame;
            frame += frames;

            while (--frames>=0)
            {
                VOICE *V = synthData.voice.data ();
                l = r = 0.0f;

                for(v=0; v<synthData.activevoices; v++)
                {
                    float osc1 = V->Osc1 * sgn(std::sin(V->Osc1Phase));
                    float osc2 = V->Osc2 * std::sin(V->Osc2Phase);
                    V->Osc1Phase += V->delta * 2;
                    V->Osc2Phase += V->delta;
                    float out = (osc1 + osc2) * V->env;
                    l = out;
                    r = out;
                }
                *out0++ = l;
                *out1++ = r;
            }

            if (frame<sampleFrames)
            {
                noteEvent (synthData.events[synthData.eventPos]);
                ++synthData.eventPos;
            }
        }
    }

    for(v=0; v<synthData.activevoices; v++) if (synthData.voice[v].env < 0.00001f) synthData.voice[v] = synthData.voice[--synthData.activevoices];
}
void CTeodorSynth3001Processor::setParameter (Steinberg::Vst::ParamID index, Steinberg::Vst::ParamValue newValue, int32 sampleOffset) {

}

//------------------------------------------------------------------------
} // namespace ts3k1
