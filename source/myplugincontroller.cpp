//------------------------------------------------------------------------
// Copyright(c) 2024 My Plug-in Company.
//------------------------------------------------------------------------

#include "myplugincontroller.h"
#include "myplugincids.h"
#include "base/source/fstreamer.h"
#include "params.h"
#include "vstgui/plugin-bindings/vst3editor.h"

using namespace Steinberg;

namespace MyCompanyName {

//------------------------------------------------------------------------
// TeodorSynth3001Controller Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Controller::initialize (FUnknown* context)
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
tresult PLUGIN_API TeodorSynth3001Controller::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Controller::setComponentState (IBStream* state)
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
tresult PLUGIN_API TeodorSynth3001Controller::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TeodorSynth3001Controller::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API TeodorSynth3001Controller::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
		auto* view = new VSTGUI::VST3Editor (this, "view", "myplugineditor.uidesc");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
} // namespace MyCompanyName
