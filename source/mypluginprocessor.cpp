//------------------------------------------------------------------------
// Copyright(c) 2024 My Plug-in Company.
//------------------------------------------------------------------------

#include "mypluginprocessor.h"
#include "myplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/hosting/eventlist.h"

#include "common.hpp"
#include <cmath>
#include <iostream>

using namespace Steinberg;

namespace MyCompanyName {
//------------------------------------------------------------------------
// TeodorSynth3001Processor
//------------------------------------------------------------------------
TeodorSynth3001Processor::TeodorSynth3001Processor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kTeodorSynth3001ControllerUID);
}

//------------------------------------------------------------------------
TeodorSynth3001Processor::~TeodorSynth3001Processor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Processor::initialize (FUnknown* context)
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
	// addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Processor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Processor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Processor::process (Vst::ProcessData& data)
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
                switch (paramQueue->getParameterId ())
                {
                    case kOsc1:
                        Osc1 = static_cast<double>(value);
                        break;
                    case kOsc2:
                        Osc2 = static_cast<double>(value);
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
                // TODO: make more sense of this code
                switch (event.type) {
                    case Vst::Event::kNoteOnEvent:
                        masterOscFrequency = 440. * std::pow(2., (event.noteOn.pitch - 69.) / 12.);
                        masterOscDeltaAngle = masterOscFrequency / data.processContext->sampleRate * f2PI;
                        masterVolume = 0.3;
                        Osc1Phase = 0.;
                        Osc2Phase = 0.;
                        break;
                    case Vst::Event::kNoteOffEvent:
                        masterVolume = 0.;
                        break;
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
            for (int32 sample = 0; sample < data.numSamples; sample++)
            {
                float osc1 = Osc1 * sgn(std::sin(Osc1Phase));
                float osc2 = Osc2 * std::sin(Osc2Phase);
                Osc1Phase += masterOscDeltaAngle * 2;
                Osc2Phase += masterOscDeltaAngle;
                float out = (osc1 + osc2) * masterVolume;
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
tresult PLUGIN_API TeodorSynth3001Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Processor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Processor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);

	double fval;
	if (!streamer.readDouble(fval)) return kResultFalse;
	Osc1 = fval;
	if (!streamer.readDouble(fval)) return kResultFalse;
	Osc2 = fval;
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Processor::getState (IBStream* state)
{
	// here we need to save the model
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	streamer.writeDouble(Osc1);
	streamer.writeDouble(Osc2);

	return kResultOk;

}

//------------------------------------------------------------------------
} // namespace MyCompanyName
