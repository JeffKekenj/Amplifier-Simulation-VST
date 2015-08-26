#ifndef __sdeditor__
#include "sdeditor.h"
#endif

#ifndef __adelay__
#include "../adelay.h"
#endif

#include <stdio.h>

//-----------------------------------------------------------------------------
// resource id's
enum {
	// bitmaps
	kBackgroundId = 128,
	kFaderBodyId,
	kFaderHandleId,
	
	// positions
	kFaderX = 170,
	kFaderY = 165,

	kFaderInc = 40,

	kDisplayX = 162,
	kDisplayY = 300,
	kDisplayXWidth = 30,
	kDisplayHeight = 14
};

//-----------------------------------------------------------------------------
// prototype string convert float -> percent
void percentStringConvert (float value, char* string);
void percentStringConvert (float value, char* string)
{
	 sprintf (string, "%d%%", (int)(100 * value + 0.5f));
}


//-----------------------------------------------------------------------------
// SDEditor class implementation
//-----------------------------------------------------------------------------
SDEditor::SDEditor (AudioEffect *effect) : AEffGUIEditor (effect) 
{
	//Fader initializations
	delayFader    = 0;
	feedbackFader = 0;
	volumeFader   = 0;

	testFader =  0;

	bassFader = 0;
	midFader = 0;
	trebFader = 0;

	quantizeFader = 0;
	frequencyFader = 0;
	truncationFader = 0;

	//Display initializations
	delayDisplay  = 0;
	feedbackDisplay = 0;
	volumeDisplay = 0;

	testDisplay = 0;

	bassDisplay = 0;
	midDisplay = 0;
	trebDisplay = 0;

	quantizeDisplay = 0;
	frequencyDisplay = 0;
	truncationDisplay = 0;

	// load the background bitmap
	// we don't need to load all bitmaps, this could be done when open is called
	hBackground = new CBitmap (kBackgroundId);

	// init the size of the plugin
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = (short)hBackground->getWidth ();
	rect.bottom = (short)hBackground->getHeight ();
}

//-----------------------------------------------------------------------------
SDEditor::~SDEditor ()
{
	// free the background bitmap
	if (hBackground)
		hBackground->forget ();
	hBackground = 0;
}

//-----------------------------------------------------------------------------
bool SDEditor::open (void *ptr)
{
	// !!! always call this !!!
	AEffGUIEditor::open (ptr);
	
	//--load some bitmaps
	CBitmap* hFaderBody   = new CBitmap (kFaderBodyId);
	CBitmap* hFaderHandle = new CBitmap (kFaderHandleId);

	//--init background frame-----------------------------------------------
	// We use a local CFrame object so that calls to setParameter won't call into objects which may not exist yet. 
	// If all GUI objects are created we assign our class member to this one. See bottom of this method.
	CRect size (0, 0, hBackground->getWidth (), hBackground->getHeight ());
	CFrame* lFrame = new CFrame (size, ptr, this);
	lFrame->setBackground (hBackground);

	//--init the faders------------------------------------------------
	int minPos = kFaderY;
	int maxPos = kFaderY + hFaderBody->getHeight () - hFaderHandle->getHeight () - 1;
	CPoint point (0, 0);
	CPoint offset (1, 0);

	// Delay
	size (kFaderX, kFaderY,
          kFaderX + hFaderBody->getWidth (), kFaderY + hFaderBody->getHeight ());
	delayFader = new CVerticalSlider (size, this, kDelay, minPos, maxPos, hFaderHandle, hFaderBody, point);
	delayFader->setOffsetHandle (offset);
	delayFader->setValue (effect->getParameter (kDelay));
	lFrame->addView (delayFader);

	// FeedBack
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	feedbackFader = new CVerticalSlider (size, this, kFeedBack, minPos, maxPos, hFaderHandle, hFaderBody, point);
	feedbackFader->setOffsetHandle (offset);
	feedbackFader->setValue (effect->getParameter (kFeedBack));
	lFrame->addView (feedbackFader);

	// Volume
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	volumeFader = new CVerticalSlider (size, this, kOut, minPos, maxPos, hFaderHandle, hFaderBody, point);
	volumeFader->setOffsetHandle (offset);
	volumeFader->setValue (effect->getParameter (kOut));
	volumeFader->setDefaultValue (0.75f);
	lFrame->addView (volumeFader);

	//Test
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	testFader = new CVerticalSlider (size, this, kTest, minPos, maxPos, hFaderHandle, hFaderBody, point);
	testFader->setOffsetHandle (offset);
	testFader->setValue (effect->getParameter (kTest));
	testFader->setDefaultValue (0.5f);
	lFrame->addView (testFader);

	//Bass

	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	bassFader = new CVerticalSlider (size, this, kBass, minPos, maxPos, hFaderHandle, hFaderBody, point);
	bassFader->setOffsetHandle (offset);
	bassFader->setValue (effect->getParameter (kBass));
	bassFader->setDefaultValue (0.75f);
	lFrame->addView (bassFader);

	//Mid

	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	midFader = new CVerticalSlider (size, this, kMid, minPos, maxPos, hFaderHandle, hFaderBody, point);
	midFader->setOffsetHandle (offset);
	midFader->setValue (effect->getParameter (kMid));
	midFader->setDefaultValue (0.5f);
	lFrame->addView (midFader);

	//Treb

	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	trebFader = new CVerticalSlider (size, this, kTreb, minPos, maxPos, hFaderHandle, hFaderBody, point);
	trebFader->setOffsetHandle (offset);
	trebFader->setValue (effect->getParameter (kTreb));
	trebFader->setDefaultValue (0.5f);
	lFrame->addView (trebFader);

	//Quantize

	//size.offset (-100, 0);
	size.offset ((kFaderInc + hFaderBody->getWidth ())+15, 0);	
	quantizeFader = new CVerticalSlider (size, this, kQuantize, minPos, maxPos, hFaderHandle, hFaderBody, point);
	quantizeFader->setOffsetHandle (offset);
	quantizeFader->setValue (effect->getParameter (kQuantize));
	quantizeFader->setDefaultValue (0.0f);
	lFrame->addView (quantizeFader);

	//Frequency

	size.offset ((kFaderInc + hFaderBody->getWidth ())+28, 0);	
	frequencyFader = new CVerticalSlider (size, this, kFreq, minPos, maxPos, hFaderHandle, hFaderBody, point);
	frequencyFader->setOffsetHandle (offset);
	frequencyFader->setValue (effect->getParameter (kFreq));
	frequencyFader->setDefaultValue (0.0f);
	lFrame->addView (frequencyFader);

	//Truncation

	size.offset ((kFaderInc + hFaderBody->getWidth ())+30, 0);	
	truncationFader = new CVerticalSlider (size, this, kTrunc, minPos, maxPos, hFaderHandle, hFaderBody, point);
	truncationFader->setOffsetHandle (offset);
	truncationFader->setValue (effect->getParameter (kTrunc));
	truncationFader->setDefaultValue (0.0f);
	lFrame->addView (truncationFader);

	//--init the display------------------------------------------------
	// Delay
	size (kDisplayX, kDisplayY,
          kDisplayX + kDisplayXWidth, kDisplayY + kDisplayHeight);
	delayDisplay = new CParamDisplay (size, 0, kCenterText);
	delayDisplay->setFont (kNormalFontSmall);
	delayDisplay->setFontColor (kWhiteCColor);
	delayDisplay->setBackColor (kBlackCColor);
	delayDisplay->setFrameColor (kBlueCColor);
	delayDisplay->setValue (effect->getParameter (kDelay));
	lFrame->addView (delayDisplay);

	// FeedBack
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	feedbackDisplay = new CParamDisplay (size, 0, kCenterText);
	feedbackDisplay->setFont (kNormalFontSmall);
	feedbackDisplay->setFontColor (kWhiteCColor);
	feedbackDisplay->setBackColor (kBlackCColor);
	feedbackDisplay->setFrameColor (kBlueCColor);
	feedbackDisplay->setValue (effect->getParameter (kFeedBack));
	feedbackDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (feedbackDisplay);

	// Volume
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	volumeDisplay = new CParamDisplay (size, 0, kCenterText);
	volumeDisplay->setFont (kNormalFontSmall);
	volumeDisplay->setFontColor (kWhiteCColor);
	volumeDisplay->setBackColor (kBlackCColor);
	volumeDisplay->setFrameColor (kBlueCColor);
	volumeDisplay->setValue (effect->getParameter (kOut));
	volumeDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (volumeDisplay);

	// Test
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	testDisplay = new CParamDisplay (size, 0, kCenterText);
	testDisplay->setFont (kNormalFontSmall);
	testDisplay->setFontColor (kWhiteCColor);
	testDisplay->setBackColor (kBlackCColor);
	testDisplay->setFrameColor (kBlueCColor);
	testDisplay->setValue (effect->getParameter (kTest));
	testDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (testDisplay);

	// Bass
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	bassDisplay = new CParamDisplay (size, 0, kCenterText);
	bassDisplay->setFont (kNormalFontSmall);
	bassDisplay->setFontColor (kWhiteCColor);
	bassDisplay->setBackColor (kBlackCColor);
	bassDisplay->setFrameColor (kBlueCColor);
	bassDisplay->setValue (effect->getParameter (kBass));
	bassDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (bassDisplay);

	//Mid
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	midDisplay = new CParamDisplay (size, 0, kCenterText);
	midDisplay->setFont (kNormalFontSmall);
	midDisplay->setFontColor (kWhiteCColor);
	midDisplay->setBackColor (kBlackCColor);
	midDisplay->setFrameColor (kBlueCColor);
	midDisplay->setValue (effect->getParameter (kMid));
	midDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (midDisplay);

	// Treb
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	trebDisplay = new CParamDisplay (size, 0, kCenterText);
	trebDisplay->setFont (kNormalFontSmall);
	trebDisplay->setFontColor (kWhiteCColor);
	trebDisplay->setBackColor (kBlackCColor);
	trebDisplay->setFrameColor (kBlueCColor);
	trebDisplay->setValue (effect->getParameter (kTreb));
	trebDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (trebDisplay);

	// Quantize
	size.offset ((kFaderInc + hFaderBody->getWidth ())+15, 0);
	quantizeDisplay = new CParamDisplay (size, 0, kCenterText);
	quantizeDisplay->setFont (kNormalFontSmall);
	quantizeDisplay->setFontColor (kWhiteCColor);
	quantizeDisplay->setBackColor (kBlackCColor);
	quantizeDisplay->setFrameColor (kBlueCColor);
	quantizeDisplay->setValue (effect->getParameter (kQuantize));
	quantizeDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (quantizeDisplay);

	// Frequency
	size.offset ((kFaderInc + hFaderBody->getWidth ())+28, 0);
	frequencyDisplay = new CParamDisplay (size, 0, kCenterText);
	frequencyDisplay->setFont (kNormalFontSmall);
	frequencyDisplay->setFontColor (kWhiteCColor);
	frequencyDisplay->setBackColor (kBlackCColor);
	frequencyDisplay->setFrameColor (kBlueCColor);
	frequencyDisplay->setValue (effect->getParameter (kFreq));
	frequencyDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (frequencyDisplay);

	// Truncation
	size.offset ((kFaderInc + hFaderBody->getWidth ())+30, 0);
	truncationDisplay = new CParamDisplay (size, 0, kCenterText);
	truncationDisplay->setFont (kNormalFontSmall);
	truncationDisplay->setFontColor (kWhiteCColor);
	truncationDisplay->setBackColor (kBlackCColor);
	truncationDisplay->setFrameColor (kBlueCColor);
	truncationDisplay->setValue (effect->getParameter (kTrunc));
	truncationDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (truncationDisplay);


	// Note : in the constructor of a CBitmap, the number of references is set to 1.
	// Then, each time the bitmap is used (for hinstance in a vertical slider), this
	// number is incremented.
	// As a consequence, everything happens as if the constructor by itself was adding
	// a reference. That's why we need til here a call to forget ().
	// You mustn't call delete here directly, because the bitmap is used by some CControls...
	// These "rules" apply to the other VSTGUI objects too.
	hFaderBody->forget ();
	hFaderHandle->forget ();

	frame = lFrame;
	return true;
}

//-----------------------------------------------------------------------------
void SDEditor::close ()
{
	delete frame;
	frame = 0;
}

//-----------------------------------------------------------------------------
void SDEditor::setParameter (VstInt32 index, float value)
{
	if (frame == 0)
		return;

	// called from ADelayEdit
	switch (index)
	{
		case kDelay:
			if (delayFader)
				delayFader->setValue (effect->getParameter (index));
			if (delayDisplay)
				delayDisplay->setValue (effect->getParameter (index));
			break;

		case kFeedBack:
			if (feedbackFader)
				feedbackFader->setValue (effect->getParameter (index));
			if (feedbackDisplay)
				feedbackDisplay->setValue (effect->getParameter (index));
			break;

		case kOut:
			if (volumeFader)
				volumeFader->setValue (effect->getParameter (index));
			if (volumeDisplay)
				volumeDisplay->setValue (effect->getParameter (index));
			break;

		case kTest:
			if (testFader)
				testFader->setValue (effect->getParameter (index));
			if (testDisplay)
				testDisplay->setValue (effect->getParameter (index));
			break;

		case kBass:
			if (bassFader)
				bassFader->setValue (effect->getParameter (index));
			if (bassDisplay)
				bassDisplay->setValue (effect->getParameter (index));
			break;

		case kMid:
			if (midFader)
				midFader->setValue (effect->getParameter (index));
			if (midDisplay)
				midDisplay->setValue (effect->getParameter (index));
			break;

		case kTreb:
			if (trebFader)
				trebFader->setValue (effect->getParameter (index));
			if (trebDisplay)
				trebDisplay->setValue (effect->getParameter (index));
			break;

		case kQuantize:
			if (quantizeFader)
				quantizeFader->setValue (effect->getParameter (index));
			if (quantizeDisplay)
				quantizeDisplay->setValue (effect->getParameter (index));
			break;

		case kFreq:
			if (frequencyFader)
				frequencyFader->setValue (effect->getParameter (index));
			if (frequencyDisplay)
				frequencyDisplay->setValue (effect->getParameter (index));
			break;

		case kTrunc:
			if (truncationFader)
				truncationFader->setValue (effect->getParameter (index));
			if (truncationDisplay)
				truncationDisplay->setValue (effect->getParameter (index));
			break;

	}
}

//-----------------------------------------------------------------------------
void SDEditor::valueChanged (CDrawContext* context, CControl* control)
{
	long tag = control->getTag ();
	switch (tag)
	{
		case kDelay:
		case kFeedBack:
		case kOut:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		case kTest:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		case kBass:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		case kMid:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		case kTreb:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		case kQuantize:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		case kFreq:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		case kTrunc:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		break;
	}
}