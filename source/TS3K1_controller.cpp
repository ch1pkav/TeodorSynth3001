//------------------------------------------------------------------------
// Copyright(c) 2024 Teodor Synths.
//------------------------------------------------------------------------

#include <base/source/fstreamer.h>
#include "TS3K1_controller.h"
#include "TS3K1_cids.h"
#include "params.h"


using namespace Steinberg;

namespace ts3k1 {

//------------------------------------------------------------------------
// CTeodorSynth3001Controller Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Controller::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// Here you could register some parameters
    Steinberg::Vst::EditController::setKnobMode(Vst::kLinearMode);
    parameters.addParameter(STR16("Osc1"), nullptr, 0, defaultOsc1, Vst::ParameterInfo::kCanAutomate, kOsc1);
    parameters.addParameter(STR16("Osc2"), nullptr, 0, defaultOsc2, Vst::ParameterInfo::kCanAutomate, kOsc2);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Controller::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Controller::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);
    double fval;

    if (!streamer.readDouble(fval))
        return kResultFalse;
    setParamNormalized(kOsc1, fval);
    if(!streamer.readDouble(fval))
        return kResultFalse;
    setParamNormalized(kOsc2, fval);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Controller::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Controller::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API CTeodorSynth3001Controller::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
        return nullptr;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Controller::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	// called by host to update your parameters
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Controller::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	// called by host to get a string for given normalized value of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API CTeodorSynth3001Controller::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	// called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
} // namespace ts3k1
