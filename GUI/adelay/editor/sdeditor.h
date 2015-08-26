//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:28 $
//
// Category     : VST 2.x SDK Samples
// Filename     : sdeditor.h
// Created by   : Steinberg Media Technologies
// Description  : Simple Surround Delay plugin with Editor using VSTGUI
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __sdeditor__
#define __sdeditor__


// include VSTGUI
#ifndef __vstgui__
#include "H:\Other\COSC4P98\VST_SDK_24\VST SDK\vst_sdk2_4_rev2\vstsdk2.4\vstgui.sf\vstgui\vstgui.h"
#endif


//-----------------------------------------------------------------------------
class SDEditor : public AEffGUIEditor, public CControlListener
{
public:
	SDEditor (AudioEffect* effect);
	virtual ~SDEditor ();

public:
	virtual bool open (void* ptr);
	virtual void close ();

	virtual void setParameter (VstInt32 index, float value);
	virtual void valueChanged (CDrawContext* context, CControl* control);

private:
	// Controls
	CVerticalSlider* delayFader;
	CVerticalSlider* feedbackFader;
	CVerticalSlider* volumeFader;

	CVerticalSlider* testFader;

	CVerticalSlider* bassFader;
	CVerticalSlider* midFader;
	CVerticalSlider* trebFader;

	CVerticalSlider* quantizeFader;
	CVerticalSlider* frequencyFader;
	CVerticalSlider* truncationFader;


	CParamDisplay* delayDisplay;
	CParamDisplay* feedbackDisplay;
	CParamDisplay* volumeDisplay;

	CParamDisplay* testDisplay;

	CParamDisplay* bassDisplay;
	CParamDisplay* midDisplay;
	CParamDisplay* trebDisplay;

	CParamDisplay* quantizeDisplay;
	CParamDisplay* frequencyDisplay;
	CParamDisplay* truncationDisplay;


	// Bitmap
	CBitmap* hBackground;
};

#endif
