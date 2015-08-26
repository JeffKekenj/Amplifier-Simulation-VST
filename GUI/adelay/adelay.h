//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : adelay.h
// Created by   : Steinberg Media Technologies
// Description  : Simple Delay plugin (Mono->Stereo)
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

/*COSC 4P98 Project
Jeff Kekenj 4759171*/

#ifndef __adelay__
#define __adelay__

#include "C:/public.sdk/source/vst2.x/audioeffectx.h"

typedef struct {
  // Filter #1 (Low band) 
  // Frequency
  // Poles
  double  lf;       
  double  f1p0;     
  double  f1p1;
  double  f1p2;
  double  f1p3;
 
  // Filter #2 (High band) 
  double  hf;       
  double  f2p0;     
  double  f2p1;
  double  f2p2;
  double  f2p3;
 
  //Track history buffer 
  double  sdm1;    
  double  sdm2;     
  double  sdm3;     
 
  // Gain Controls 
  double  lg;      
  double  mg;      
  double  hg;     
} EQSTATE;

//Parameter Declarations
enum
{
	// Global
	kNumPrograms = 16,

	// Parameters Tags
	kDelay = 0,
	kFeedBack,
	kOut,
	kTest,
	kBass,
	kMid,
	kTreb,
	kQuantize,
	kFreq,
	kTrunc,

	kNumParams
};

class ADelay;

//------------------------------------------------------------------------
class ADelayProgram
{
friend class ADelay;
public:
	ADelayProgram ();
	~ADelayProgram () {}

private:	
	float fDelay;
	float fFeedBack;
	float fOut;
	float fTest;
	float fBass;
	float fMid;
	float fTreb;
	float fQuant;
	float fFreq;
	float fTrunc;
	char name[24];
};

//------------------------------------------------------------------------
class ADelay : public AudioEffectX
{
public:
	ADelay (audioMasterCallback audioMaster);
	~ADelay ();

	//---from AudioEffect-----------------------
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

	virtual void setProgram (VstInt32 program);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual void resume ();

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

protected:
	void setDelay (float delay);

	ADelayProgram* programs;
	
	float* buffer;
	float fDelay;
	float fFeedBack;
	float fOut;
	float fTest;
	float fBass;
	float fMid;
	float fTreb;
	float fQuant;
	float fFreq;
	float fTrunc;
	
	long delay;
	long size;
	long cursor;
};

#endif
