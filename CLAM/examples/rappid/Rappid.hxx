/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _RAPPID_H
#define _RAPPID_H

#include "ProcessingComposite.hxx"
#include "RappidInput.hxx"
#include "RappidOutput.hxx"
#include "RappidTypes.hxx"
#include "EnvelopeMorpher.hxx"
#include "DynamicMixer.hxx"
#include "Controller.hxx"
#include "RappidConfig.hxx"
#include "Watchdog.hxx"
#include <pthread.h>

using namespace CLAM;

class Rappid: public ProcessingComposite {

public:

	typedef enum {
		eFirstControl=0,         // Always first
		eEnableEM1   =0,         // Always second
		eEM1InterpolationPeriod,
		eEM1IntegrationLength,
		eEM1NormalLevel,
		eEM1SilenceLevel,
		eEnableEM2,
		eEM2InterpolationPeriod,
		eEM2IntegrationLength,
		eEM2NormalLevel,
		eEM2SilenceLevel,
		eNUMControls             // Always last
	} EControls;

	Controller Control;

	Rappid(const RappidConfig& cfg=RappidConfig());

	bool StartRunning();

	bool StopRunning();

	/** This method is some kind of application loop method. 
		It waits forever until the processing thread dies.
		As a side effect, 
	 */
	void WaitWhileProcessing();

	bool Do(void);

	const ProcessingConfig &GetConfig() const {return mConfig;}

	const RappidConfig     &GetConfiguration() const {return mConfig;}

private:

	RappidConfig            mConfig;
	void Reconfigure();

	RappidInput             mViola;
	RappidInput             mHarp;

	RappidOutput            mSynthL;
	RappidOutput            mSynthR;

	EnvelopeMorpher         mEM1;
	EnvelopeMorpher         mEM2;

	CLAM::Audio             mViolaAudio;
	CLAM::Audio             mHarpAudio;

	CLAM::Audio             mSynthAudioL;
	CLAM::Audio             mSynthAudioR;

	CLAM::Audio             mEM1OutputAudio;
	CLAM::Audio             mEM2OutputAudio;

	DynamicMixer<1>         mMixerL;
	DynamicMixer<1>         mMixerR;

	CLAM::AudioFileOut      mDebugOutput;

	Watchdog                mWatchdog;

	TTime mTime;
	TTime mTimeLimit;
	TTime mFrameTime;
	TData mSampleRate;

	bool            mDebug;
	bool            mRunOnce;
	bool            mStopRequest;

	pthread_t       mProcessingThread;
	pthread_mutex_t mProcessingMutex;

	pthread_mutex_t mRunningConditionMutex;
	pthread_cond_t  mRunningCondition;

	static void* ProcessingThread(void*);

	void AdoptChildren();

	void AttachChildren();

	bool ConfigureChildren();

	void ControlSetup();

	void InitializeControlValues();

	void ConfigureAudio(Audio&);

	bool ConfigureData();

	void InitializeData();

	bool ConcreteConfigure(const ProcessingConfig&);

	bool ConcreteStart();

};

#endif
