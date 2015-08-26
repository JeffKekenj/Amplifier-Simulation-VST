//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : adelay.cpp
// Created by   : Steinberg Media Technologies
// Description  : Simple Delay plugin (Mono->Stereo)
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

/*COSC 4P98 Project
Jeff Kekenj 4759171
Distortion/ Amp simulation, 3-band eq*/

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include<conio.h>
#include<stdlib.h>

#include <iostream>
#include <fstream>


#ifndef __adelay__
#include "adelay.h"
#endif


static double vsa = (1.0 / 4294967295.0);   // Very small amount (Denormal Fix) for eq
double count=0;//for freq distortion counter

//----------------------------------------------------------------------------- 
ADelayProgram::ADelayProgram ()
{
	// default Program Values
	//Program is initialized to values which do not cause clipping
	fDelay = 0.1;
	fFeedBack = 1;
	fOut = 1;
	fTest = 0.5;

	fBass = 1;
	fMid = 1;
	fTreb = 1;

	fQuant = 1;
	fFreq = 0;
	fTrunc = 1;

	strcpy (name, "Init");
}

//-----------------------------------------------------------------------------
ADelay::ADelay (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init all parameter values
	size = 44100;
	cursor = 0;
	delay = 0;
	buffer = new float[size];
	
	programs = new ADelayProgram[numPrograms];
	fDelay = fFeedBack = fOut = fTest = 0;

	if (programs)
		setProgram (0);

	setNumInputs (1);	// mono input
	setNumOutputs (2);	// stereo output

	setUniqueID ('ADly');
	resume ();		// flush buffer
}

//------------------------------------------------------------------------
ADelay::~ADelay ()
{
	if (buffer)
		delete[] buffer;
	if (programs)
		delete[] programs;
}

//------------------------------------------------------------------------
//Set program values from slider/gui values
void ADelay::setProgram (VstInt32 program)
{
	ADelayProgram* ap = &programs[program];

	curProgram = program;
	setParameter (kDelay, ap->fDelay);	
	setParameter (kFeedBack, ap->fFeedBack);
	setParameter (kOut, ap->fOut);
	setParameter (kTest, ap->fTest);

	setParameter (kBass, ap->fBass);
	setParameter (kMid, ap->fMid);
	setParameter (kTreb, ap->fTreb);

	setParameter (kQuantize, ap->fQuant);
	setParameter (kFreq, ap->fFreq);
	setParameter (kTrunc, ap->fTrunc);
}

//------------------------------------------------------------------------
void ADelay::setDelay (float fdelay)
{
	fDelay = fdelay;
	programs[curProgram].fDelay = fdelay;
	cursor = 0;
	delay = (long)(fdelay * (float)(size - 1));
}

//------------------------------------------------------------------------
void ADelay::setProgramName (char *name)
{
	strcpy (programs[curProgram].name, name);
}

//------------------------------------------------------------------------
void ADelay::getProgramName (char *name)
{
	if (!strcmp (programs[curProgram].name, "Init"))
		sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
	else
		strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool ADelay::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, programs[index].name);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
void ADelay::resume ()
{
	memset (buffer, 0, size * sizeof (float));
	AudioEffectX::resume ();
}

//------------------------------------------------------------------------
//set local k Values to the f values from the VST GUI
void ADelay::setParameter (VstInt32 index, float value)
{
	ADelayProgram* ap = &programs[curProgram];

	switch (index)
	{
		case kDelay :    setDelay (value);					break;
		case kFeedBack : fFeedBack = ap->fFeedBack = value; break;
		case kOut :      fOut = ap->fOut = value;			break;
		case kTest :      fTest = ap->fTest = value;			break;
		case kBass :      fBass = ap->fBass = value;			break;
		case kMid :		  fMid = ap->fMid = value;			break;
		case kTreb :      fTreb = ap->fTreb = value;			break;
		case kQuantize :      fQuant = ap->fQuant = value;			break;
		case kFreq :     fFreq = ap->fFreq = value;			break;
		case kTrunc:     fTrunc = ap->fTrunc = value;			break;
	}
}

//------------------------------------------------------------------------
//get parameter from VST GUI
float ADelay::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kDelay :    v = fDelay;	break;
		case kFeedBack : v = fFeedBack; break;
		case kOut :      v = fOut;		break;
		case kTest :      v = fTest;		break;
		case kBass :      v = fBass;		break;
		case kMid :      v = fMid;		break;
		case kTreb :      v = fTreb;		break;
		case kQuantize :      v = fQuant;		break;
		case kFreq :      v = fFreq;		break;
		case kTrunc :      v = fTrunc;		break;
	}
	return v;
}

//------------------------------------------------------------------------
//Get parameter name to display
void ADelay::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kDelay :    strcpy (label, "Delay");		break;
		case kFeedBack : strcpy (label, "FeedBack");	break;
		case kOut :      strcpy (label, "Volume");		break;
		case kTest :      strcpy (label, "Test");		break;
		case kBass :      strcpy (label, "Bass");		break;
		case kMid :      strcpy (label, "Mid");		break;
		case kTreb :      strcpy (label, "Treb");		break;
		case kQuantize :      strcpy (label, "Quant");		break;
		case kFreq :      strcpy (label, "Freq");		break;
		case kTrunc :      strcpy (label, "Trunc");		break;
	}
}

//------------------------------------------------------------------------
void ADelay::getParameterDisplay (VstInt32 index, char *text)
{
	switch (index)
	{
		case kDelay :    int2string (delay, text, kVstMaxParamStrLen);			break;
		case kFeedBack : float2string (fFeedBack, text, kVstMaxParamStrLen);	break;
		case kOut :      dB2string (fOut, text, kVstMaxParamStrLen);			break;
		case kTest :      dB2string (fTest, text, kVstMaxParamStrLen);			break;
		case kBass :      dB2string (fBass, text, kVstMaxParamStrLen);			break;
		case kMid :      dB2string (fMid, text, kVstMaxParamStrLen);			break;
		case kTreb :      dB2string (fTreb, text, kVstMaxParamStrLen);			break;
		case kQuantize :      dB2string (fQuant, text, kVstMaxParamStrLen);			break;
		case kFreq :      dB2string (fFreq, text, kVstMaxParamStrLen);			break;
		case kTrunc :      dB2string (fTrunc, text, kVstMaxParamStrLen);			break;
	}
}

//------------------------------------------------------------------------
void ADelay::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kDelay :    strcpy (label, "samples");	break;
		case kFeedBack : strcpy (label, "amount");	break;
		case kOut :      strcpy (label, "dB");		break;
		case kTest :      strcpy (label, "dB");		break;
		case kBass :      strcpy (label, "dB");		break;
		case kMid :      strcpy (label, "dB");		break;
		case kTreb :      strcpy (label, "dB");		break;
		case kQuantize :      strcpy (label, "dB");		break;
		case kFreq :      strcpy (label, "dB");		break;
		case kTrunc :      strcpy (label, "dB");		break;
	}
}

//------------------------------------------------------------------------
bool ADelay::getEffectName (char* name)
{
	strcpy (name, "Distortion");
	return true;
}

//------------------------------------------------------------------------
bool ADelay::getProductString (char* text)
{
	strcpy (text, "Distortion");
	return true;
}

//------------------------------------------------------------------------
bool ADelay::getVendorString (char* text)
{
	strcpy (text, "Steinberg Media Technologies");
	return true;
}

//Returns the a lower volume for teh sample provided
double semiLow (double temp){
	return temp*0.001;
}

//Source http://learnvst.wordpress.com/old-site/weq-plugin-project/
double threeBandEQ(EQSTATE* es, double sample){
	  double  l,m,h;      // Low / Mid / High - Sample Values
 
	  // Filter #1 (lowpass) 
	  es->f1p0  += (es->lf * (sample   - es->f1p0)) + vsa;
	  es->f1p1  += (es->lf * (es->f1p0 - es->f1p1));
	  es->f1p2  += (es->lf * (es->f1p1 - es->f1p2));
	  es->f1p3  += (es->lf * (es->f1p2 - es->f1p3));
 
	  l  = es->f1p3;
 
	  // Filter #2 (highpass) 
	  es->f2p0  += (es->hf * (sample   - es->f2p0)) + vsa;
	  es->f2p1  += (es->hf * (es->f2p0 - es->f2p1));
	  es->f2p2  += (es->hf * (es->f2p1 - es->f2p2));
	  es->f2p3  += (es->hf * (es->f2p2 - es->f2p3));
 
	  h = es->sdm3 - es->f2p3;
 
	  // Calculate midrange (signal - (low + high)) 
	  m          = es->sdm3 - (h + l);
 
	  // Scale, Combine and store 
	  l         *= es->lg;
	  m         *= es->mg;
	  h         *= es->hg;
 
	  // Shuffle history buffer 
	  es->sdm3   = es->sdm2;
	  es->sdm2   = es->sdm1;
	  es->sdm1   = sample;
  
	  //return sum of low mid and high	
	  return(l+m+h);
}

	//Quantize function, reduces number of bits per sample
	//to create distortion
	double quantize(double sample, int bits) {
		long int output = 0;
		bits = bits-1;
		if (bits == 16){
			return sample;
		} if (bits < 0.7){
			return 0;
		} else {			
			output = 1<<(bits);
			count = count + 0.5;
			if (count>=1) {
				count = count-1;
				return (long int)(sample*output)/(float)output;
			}
		}
	}

	int wav = -10;
	int increment;
	//sine wav to control speed of playback
	//creates distortion among the bands
	double freqDistortion (double samples, double playback){		
		if (playback == 0){
			return samples;
		}
		if (wav <= 20){
			wav = (wav + (20*playback));			
		} else {
			wav = -20; 
		}
		return (((samples*wav)/playback)*0.005);
	}


//---------------------------------------------------------------------------
void ADelay::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	//initialization of local variables used for
	//working with methods on samples
	float* in = inputs[0];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	double dVolume = fDelay;
	double localNegativeClip = fFeedBack;
	double localPositiveClip = fOut;
	double postVolume = fTest;
	EQSTATE eqTest;
	int bits = 16;
	double tempL;

	localNegativeClip = localNegativeClip*10;
	localPositiveClip = localPositiveClip*10;

	double localBass = fBass;
	double localMid = fMid;
	double localTreb = fTreb;
	double localGate = fTest;
	double localRev = fQuant;
	double localFreq = fFreq;
	double localTruncation = fTrunc;

	memset(&eqTest,0,sizeof(EQSTATE));

	eqTest.lg = localBass;
	eqTest.mg = localMid;
	eqTest.hg = localTreb;

	eqTest.lf = 2 * sin(M_PI * ((double)880 / (double)480000));
	eqTest.hf = 2 * sin(M_PI * ((double)5000 / (double)480000));

	while (--sampleFrames >= 0)
	{
		float x = *in++;		
		tempL = ((x) * (dVolume*200));

		//Main method controlling everything
		//if EQ is 100% on each band bypass, else
		//calculate eq, and quantization distortion, and other effects accordingly
		if (localBass == 1 && localMid == 1  && localTreb == 1){
			if (tempL > localPositiveClip){ 
				(*out1++) = (freqDistortion(quantize((localPositiveClip*localTruncation)*postVolume,(int)(bits*localRev)), localFreq)); 
					if (out2)
						(*out2++) = quantize((localPositiveClip*localTruncation)*postVolume, (int)(bits*localRev)); 
			} else if (tempL < -localNegativeClip){
					(*out1++) = freqDistortion(quantize((-(localNegativeClip*localTruncation))*postVolume, (int)(bits*localRev)), localFreq); 
					if (out2)
						(*out2++) = freqDistortion(quantize((-(localNegativeClip*localTruncation))*postVolume, (int)(bits*localRev)), localFreq); 
				}
			 else {
				(*out1++) = freqDistortion(quantize(semiLow(tempL)*(postVolume*200), (int)(bits*localRev)), localFreq);  
				if (out2)
					(*out2++) = (freqDistortion(quantize(semiLow(tempL)*(postVolume*200), (int)(bits*localRev)), localFreq));  
			}
		} else {
			if (tempL > localPositiveClip || tempL < -localNegativeClip){
				if (tempL > localPositiveClip){ 
					(*out1++) = freqDistortion(quantize((threeBandEQ(&eqTest, (localPositiveClip*localTruncation)))*postVolume, (int)(bits*localRev)), localFreq); 
					if (out2)
						(*out2++) = freqDistortion(quantize((threeBandEQ(&eqTest, (localPositiveClip*localTruncation)))*postVolume, (int)(bits*localRev)), localFreq);  
				} else if (tempL < -localNegativeClip){
					(*out1++) = freqDistortion(quantize((-threeBandEQ(&eqTest, (localNegativeClip*localTruncation)))*postVolume, (int)(bits*localRev)), localFreq); 
					if (out2)
						(*out2++) = freqDistortion(quantize((-threeBandEQ(&eqTest, (localNegativeClip*localTruncation)))*postVolume, (int)(bits*localRev)), localFreq);  
				}
			}  else {
				(*out1++) = freqDistortion(quantize(threeBandEQ(&eqTest, (semiLow(tempL)*(postVolume*200))), (int)(bits*localRev)), localFreq);  
				if (out2)
					(*out2++) = freqDistortion(quantize(threeBandEQ(&eqTest, (semiLow(tempL)*(postVolume*200))), (int)(bits*localRev)), localFreq);  
			}
		}
	}
}
