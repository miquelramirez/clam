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


#include "UserInterface.hxx"
#include "FL/fl_file_chooser.H"
#include "Fl_Progress.cxx"
#include "Fl_WaitMessage.cxx"
#include "AnalysisSynthesisExampleBase.hxx"
#include "pthread.h"
#include "DebugSnapshots.hxx" 
#include "AudioSnapshot.hxx" 
#include "SpectrumSnapshot.hxx" 

/******* TRANSFORMATION *******/
#include "SMSFreqShift.hxx"


#include <iostream>

using namespace CLAM;
using namespace CLAMGUI;
using namespace std;



#ifndef WIN32
#include <unistd.h>
#define Sleep(var) usleep(var*1000)
#endif

#ifdef WIN32
#include <windows.h>
#endif

class ProgressGUI:public Progress
{
public:
	Fl_Progress* mFlProgress;
	static Fl_Progress* requested;
	static Fl_Progress* delRequested;
	static const char* requestedTitle;
	static pthread_t flThread;
	static float requestedFrom;	
	static float requestedTo;	
	
	void Update(float val)
	{
		mFlProgress->mValue = val;
	}

	ProgressGUI(const char* title,float from,float to)
	:Progress(title,from,to)
	{
		if (flThread == pthread_self())
		{

			mFlProgress = new Fl_Progress;
			mFlProgress->mLabel->label(mTitle);
			mFlProgress->mSlider->range(mFrom,mTo);
			mFlProgress->mWindow->show();
		}else{
			requested = 0;

			requestedFrom = mFrom;
			requestedTo = mTo;
			requestedTitle = mTitle;
			while (requested==0)
			{
				Sleep(1);
			}

			mFlProgress = requested;
		}
	}

	~ProgressGUI()
	{
		if (flThread == pthread_self())
		{

			delete mFlProgress;
		}else{
			delRequested = mFlProgress;
			while (delRequested)
			{
				Sleep(1);
			}
		}
	}	

	static void Idle(void* ptr)
	{
		if (requestedTitle)
		{

			const char* tmp = requestedTitle;
			float tmpFrom = requestedFrom;
			float tmpTo = requestedTo;
			requestedTitle = 0;
			Fl_Progress * o= new Fl_Progress;
			o->mLabel->label(tmp);
			o->mSlider->range(tmpFrom,tmpTo);
			o->mWindow->show();
			requested = o;
		}
		if (delRequested)
		{

			Fl_Progress* tmp = delRequested;
			delRequested = 0;
			delete tmp;
		}
	}
};

Fl_Progress* ProgressGUI::requested = 0;
Fl_Progress* ProgressGUI::delRequested = 0;
const char* ProgressGUI::requestedTitle = 0;
float ProgressGUI::requestedFrom;
float ProgressGUI::requestedTo;
pthread_t ProgressGUI::flThread;

class WaitMessageGUI:public WaitMessage
{
public:
	Fl_WaitMessage* mFlWaitMessage;
	static Fl_WaitMessage* requested;
	static Fl_WaitMessage* delRequested;
	static const char* requestedTitle;
	static pthread_t flThread;
	
	
	WaitMessageGUI(const char* title)
	:WaitMessage(title)
	{
		if (flThread == pthread_self())
		{
	
			mFlWaitMessage = new Fl_WaitMessage;
			mFlWaitMessage->mLabel->label(title);
			mFlWaitMessage->mWindow->show();
		}else{
			requested = 0;
	
			requestedTitle = title;
			while (requested==0)
			{
				Sleep(1);
			}
	
			mFlWaitMessage = requested;
		}
	}

	~WaitMessageGUI()
	{
		if (flThread == pthread_self())
		{

			delete mFlWaitMessage;
		}else{
			delRequested = mFlWaitMessage;
			while (delRequested)
			{
				Sleep(10);
			}
		}
	}	

	static void Idle(void* ptr)
	{
		if (requestedTitle)
		{

			const char* tmp = requestedTitle;
			requestedTitle = 0;
			Fl_WaitMessage * o= new Fl_WaitMessage;
			o->mLabel->label(tmp);
			o->mWindow->show();
			requested = o;
		}
		if (delRequested)
		{

			Fl_WaitMessage* tmp = delRequested;
			delRequested = 0;
			delete tmp;
		}
	}

};

Fl_WaitMessage* WaitMessageGUI::requested = 0;
Fl_WaitMessage* WaitMessageGUI::delRequested = 0;
const char* WaitMessageGUI::requestedTitle = 0;
pthread_t WaitMessageGUI::flThread;

/** This class adds FLTK GUI to AnalysisSynthesisExampleBase.
 *	@see AnalysisSynthesisExampleBase
 */
class AnalysisSynthesisExampleGUI:public AnalysisSynthesisExampleBase
{
public:
	friend class UserInterface;
	void Run(void)
	{
		UserInterface userinterface;
		userinterface.mAnalysisSynthesisExample = this;
		Fl::get_system_colors();
		Fl::set_boxtype(FL_UP_BOX,FL_THIN_UP_BOX);
		Fl::set_boxtype(FL_DOWN_BOX,FL_THIN_DOWN_BOX);
		userinterface.mWindow->show();
		WaitMessageGUI::flThread = pthread_self();
		ProgressGUI::flThread = pthread_self();
		Fl::add_idle(WaitMessageGUI::Idle);
		Fl::add_idle(ProgressGUI::Idle);



		Fl::run();
	}
	
	CLAM::Progress* CreateProgress(const char* title,float from,float to) 
	{
		return new ProgressGUI(title,from,to);
	}

	CLAM::WaitMessage* CreateWaitMessage(const char* title) 
	{
		return new WaitMessageGUI(title);
	}

	int mThreadState;

	static void* startfn_analyze(void* ptr)
	{
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
		((AnalysisSynthesisExampleGUI*)ptr)->Analyze();
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
		return 0;
	}

	static void* startfn_synthesize(void* ptr)
	{
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
		((AnalysisSynthesisExampleGUI*)ptr)->Synthesize();
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
		return 0;
	}

	static void* startfn_LoadAnalysis(void* ptr)
	{

		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
		((AnalysisSynthesisExampleGUI*)ptr)->LoadAnalysis(
			((AnalysisSynthesisExampleGUI*)ptr)->mFilename);
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
		return 0;
	}

	static void* startfn_StoreAnalysis(void* ptr)
	{

		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
		((AnalysisSynthesisExampleGUI*)ptr)->StoreAnalysis();
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
		return 0;
	}
	
	pthread_t mThread;
	
	void AnalyzeThread(void)
	{
		mThreadState = 0;
		pthread_create(&mThread,NULL,startfn_analyze,this);
		while (mThreadState==0) { Fl::wait(0.1); }
		while (mThreadState==1) { Fl::wait(0.1); }
		pthread_join(mThread,NULL);
	}

	void SynthesizeThread(void)
	{
		mThreadState = 0;
		pthread_create(&mThread,NULL,startfn_synthesize,this);
		while (mThreadState==0) { Fl::wait(0.1); }
		while (mThreadState==1) { Fl::wait(0.1); }
		pthread_join(mThread,NULL);
	}

	std::string mFilename;

	void LoadAnalysisThread(const std::string& filename)
	{
		mFilename = filename;
		mThreadState = 0;
		pthread_create(&mThread,NULL,startfn_LoadAnalysis,this);
		while (mThreadState==0) { Fl::wait(0.1); }
		while (mThreadState==1) { Fl::wait(0.1); }
		pthread_join(mThread,NULL);
	}

	void StoreAnalysisThread(void)
	{
		mThreadState = 0;
		pthread_create(&mThread,NULL,startfn_StoreAnalysis,this);
		while (mThreadState==0) { Fl::wait(0.1); }
		while (mThreadState==1) { Fl::wait(0.1); }
		pthread_join(mThread,NULL);
	}

};

void UserInterface::LoadConfiguration(void)
{

	char* str = fl_file_chooser("Select configuration file","*.xml","");
	
	if ( str )
		{
		
			mConfigurationText->value(str);
		
			std::string inputXMLFileName(str);
		
			mAnalysisSynthesisExample->LoadConfig(inputXMLFileName);
		
			if (mAnalysisSynthesisExample->mHaveConfig)
		   	{	
					mAnalysisSynthesisExample->LoadInputSound();
					if (mAnalysisSynthesisExample->mHaveAudioIn)
		   			{
							mDisplayInputSoundButton->activate();
							mAnalyzeButton->activate();
		   			}
					else 
					{
						mAnalysisSynthesisExample->mHaveConfig=false;
						mDisplayInputSoundButton->deactivate();
						mAnalyzeButton->deactivate();
					}
		   	}
			else 
			{
				mDisplayInputSoundButton->deactivate();
				mAnalyzeButton->deactivate();
			}
		

		if (
			mAnalysisSynthesisExample->mHaveAnalysis &&
			mAnalysisSynthesisExample->mHaveConfig)
			{
				mSynthesizeButton->activate();
			}
		}		
}

void UserInterface::LoadTransformation(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	if ( str )
		{
			mTransformationFileText->value(str);
			std::string inputXMLFileName(str);
			mAnalysisSynthesisExample->LoadTransformationScore(inputXMLFileName);
			mAnalysisSynthesisExample->mHaveTransformationScore=true;
			if (mAnalysisSynthesisExample->mHaveAnalysis)
				{
					mTransformButton->activate();
				}
		}
}

void UserInterface::LoadAnalysisData(void)
{
	char* str = fl_file_chooser("Select analysis data file","*.xml","");
	if (str)
	{
		mAnalysisDataText->value(str);
		std::string inputXMLFileName(str);
		mAnalysisSynthesisExample->LoadAnalysisThread(inputXMLFileName);
		mAnalyzeMelodyButton->deactivate();
		if (
			mAnalysisSynthesisExample->mHaveAnalysis &&
			mAnalysisSynthesisExample->mHaveConfig)
		{
			mSynthesizeButton->activate();
		}
	}
}

void UserInterface::Analyze(void)
{
	mAnalysisSynthesisExample->AnalyzeThread();
	if (mAnalysisSynthesisExample->mHaveAnalysis)
	{
		mAnalyzeMelodyButton->activate();
		mSynthesizeButton->activate();
		mStoreAnalysisDataButton->activate();
		if(mAnalysisSynthesisExample->mHaveTransformationScore)
			mTransformButton->activate();
	}
}

void UserInterface::Synthesize(void)
{
	mAnalysisSynthesisExample->SynthesizeThread();
	if (mAnalysisSynthesisExample->mHaveAudioOut)
	{
		mStoreOutputSoundButton->activate();
		mDisplayOutputSoundButton->activate();
		mStoreOutputSoundSinusoidalButton->activate();
		mDisplayOutputSoundSinusoidalButton->activate();
		mStoreOutputSoundResidualButton->activate();
		mDisplayOutputSoundResidualButton->activate();
	}
}

void UserInterface::Exit(void)
{
	delete mWindow;
}

void UserInterface::StoreAnalysisData(void)
{
	mAnalysisSynthesisExample->StoreAnalysisThread();
}

void UserInterface::DisplayInputSound(void)
{
	showSnapshotAudio(mAnalysisSynthesisExample->mAudioIn,"Input Audio");
}

void UserInterface::DisplayOutputSound(void)
{
	showSnapshotAudio(mAnalysisSynthesisExample->mAudioOut,"Output Audio");
}

void UserInterface::DisplayOutputSoundResidual(void)
{
	showSnapshotAudio(mAnalysisSynthesisExample->mAudioOutRes,"Output Audio Residual");
}

void UserInterface::DisplayOutputSoundSinusoidal(void)
{
	showSnapshotAudio(mAnalysisSynthesisExample->mAudioOutSin,"Output Audio Sinusoidal");
}

void UserInterface::StoreOutputSound(void)
{
	mAnalysisSynthesisExample->StoreOutputSound();
}

void UserInterface::StoreOutputSoundResidual(void)
{
	mAnalysisSynthesisExample->StoreOutputSoundResidual();
}

void UserInterface::StoreOutputSoundSinusoidal(void)
{
	mAnalysisSynthesisExample->StoreOutputSoundSinusoidal();
}

void UserInterface::AnalyzeMelody(void)
{
	mAnalysisSynthesisExample->AnalyzeMelody();
	mStoreMelodyButton->activate();
}

void UserInterface::StoreMelody(void)
{
	mAnalysisSynthesisExample->StoreMelody();
}

void UserInterface::Transform(void)
{
	mAnalysisSynthesisExample->Transform();
}

main(void)
{
	try{
		AnalysisSynthesisExampleGUI example;
		/******* TRANSFORMATION *******/
		SMSFreqShift myTrans;
		example.SetTransformation(&myTrans);
		/******* TRANSFORMATION *******/
		example.Run();
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	
	std::clog << "Finished successfully!";
	return 0;
}
