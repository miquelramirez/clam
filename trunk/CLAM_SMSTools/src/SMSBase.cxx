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
#include "CLAM_Math.hxx"


#include "SMSBase.hxx"
#include "SegmentDescriptors.hxx"
#include "BasicStatistics.hxx"


#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"
#include <iostream>

#include "Segment.hxx"
#include "SMSAnalysisSynthesisConfig.hxx"
#include "SMSAnalysis.hxx"
#include "SMSSynthesis.hxx"
#include "CleanTracks.hxx"
#include "Segmentator.hxx"
#include "Normalization.hxx"
#include "HeapDbg.hxx"

#include <fstream>
#include "XMLStorage.hxx"
#include "XMLStaticAdapter.hxx"
#include "XMLComponentAdapter.hxx"
#include "XMLable.hxx"

#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "AudioManager.hxx"

using namespace CLAMGUI;
using namespace CLAM;

SMSBase::SMSBase()
	: mCurrentProgressIndicator( NULL ), mCurrentWaitMessage( NULL )
{
	mHaveConfig = false;
	mHaveAnalysis = false;
	mHaveAudioIn = false;
	mHaveAudioOut = false;
	mHaveTransformationScore = false;
	mHaveMelody = false;
	mHaveSpectrum = false;
	mHaveTransformation = false;
	mHaveAudioMorph = false;

	mTransformation.mChainInput.Attach(mOriginalSegment);
	mTransformation.mChainOutput.Attach(mTransformedSegment);

}

void SMSBase::DestroyWaitMessage( )
{
	delete mCurrentWaitMessage;
	mCurrentWaitMessage = NULL;
}

void SMSBase::DestroyProgressIndicator( )
{
	delete mCurrentProgressIndicator;
	mCurrentProgressIndicator = NULL;
}

SMSBase::~SMSBase(void)
{

}

void SMSBase::InitConfigs(void)
{
	if (!mHaveConfig) return;

	/*global parameters*/
	int analWindowSize=mGlobalConfig.GetAnalysisWindowSize();
	int resAnalWindowSize=mGlobalConfig.GetResAnalysisWindowSize();

	int analHopSize;
	if(mGlobalConfig.GetAnalysisHopSize()<0)
		analHopSize=(resAnalWindowSize-1)/2;
	else
		analHopSize=mGlobalConfig.GetAnalysisHopSize();
	
	int synthFrameSize;
	if(mGlobalConfig.GetSynthesisFrameSize()<0)
		synthFrameSize=analHopSize;
	else
		synthFrameSize=mGlobalConfig.GetSynthesisFrameSize();

	int analZeroPaddingFactor=mGlobalConfig.GetAnalysisZeroPaddingFactor();
	// SMS Analysis configuration
	//if window size is even we add one !
	if (analWindowSize%2==0) analWindowSize+=1;
	if (resAnalWindowSize%2==0) resAnalWindowSize+=1;
	mAnalConfig.SetSinWindowSize(analWindowSize);
	mAnalConfig.SetHopSize(analHopSize);
	mAnalConfig.SetSinWindowType(mGlobalConfig.GetAnalysisWindowType());
	mAnalConfig.SetSinZeroPadding(analZeroPaddingFactor);
	mAnalConfig.SetResWindowSize(resAnalWindowSize);
	mAnalConfig.SetResWindowType(mGlobalConfig.GetResAnalysisWindowType());

	mAnalConfig.GetPeakDetect().SetMagThreshold(mGlobalConfig.GetAnalysisPeakDetectMagThreshold());
	
	mAnalConfig.GetSinTracking().SetnMaxSines(mGlobalConfig.GetAnalysisMaxSines());
	mAnalConfig.GetSinTracking().SetIsHarmonic(mGlobalConfig.GetAnalysisHarmonic());
	mAnalConfig.GetPeakDetect().SetMaxPeaks(mGlobalConfig.GetAnalysisMaxSines());


	mAnalConfig.GetFundFreqDetect().SetReferenceFundFreq(mGlobalConfig.GetAnalysisReferenceFundFreq());
	mAnalConfig.GetFundFreqDetect().SetLowestFundFreq(mGlobalConfig.GetAnalysisLowestFundFreq());
	mAnalConfig.GetFundFreqDetect().SetHighestFundFreq(mGlobalConfig.GetAnalysisHighestFundFreq());
	mAnalConfig.GetFundFreqDetect().SetMaxFundFreqError(mGlobalConfig.GetAnalysisMaxFundFreqError());
	mAnalConfig.GetFundFreqDetect().SetNMaxCandidates(mGlobalConfig.GetAnalysisMaxFundCandidates());
	
	//SMS Synthesis configuration
	mSynthConfig.SetAnalWindowSize(resAnalWindowSize);
	mSynthConfig.SetFrameSize(synthFrameSize);
	mSynthConfig.SetHopSize(synthFrameSize);
	mSynthConfig.GetPhaseMan().SetType(mGlobalConfig.GetSynthesisPhaseManagementType());

	
}

void SMSBase::LoadConfig(const std::string& inputFileName)
{
	if(mHaveConfig)//This means we had a previous configuration
	{
		mHaveAnalysis = false;
		mHaveAudioIn = false;
		mHaveAudioOut = false;
		mHaveTransformationScore = false;
		mHaveMelody = false;
		mHaveSpectrum = false;
		mHaveAudioMorph = false;
	}

	mCurrentWaitMessage = CreateWaitMessage( "Loading XML configuration file, please wait." );
	//Loading configuration
	XMLStorage x;
	x.Restore(mGlobalConfig,inputFileName);
	mHaveConfig = false;
	if(	
	mGlobalConfig.HasInputSoundFile() &&
	mGlobalConfig.HasOutputSoundFile() &&
	mGlobalConfig.HasOutputAnalysisFile() &&
	mGlobalConfig.HasInputAnalysisFile() &&
	mGlobalConfig.HasAnalysisWindowSize() &&
	mGlobalConfig.HasAnalysisHopSize() &&
	mGlobalConfig.HasAnalysisWindowType() &&
	mGlobalConfig.HasResAnalysisWindowSize() &&
	mGlobalConfig.HasResAnalysisWindowType() &&
	mGlobalConfig.HasAnalysisZeroPaddingFactor() &&
	mGlobalConfig.HasAnalysisPeakDetectMagThreshold() &&
	mGlobalConfig.HasAnalysisMaxSines() &&
	mGlobalConfig.HasAnalysisSinTrackingFreqDeviation() &&
	mGlobalConfig.HasAnalysisReferenceFundFreq() && 
	mGlobalConfig.HasAnalysisLowestFundFreq() && 
	mGlobalConfig.HasAnalysisHighestFundFreq() && 
	mGlobalConfig.HasAnalysisMaxFundFreqError() && 				 
	mGlobalConfig.HasAnalysisMaxFundCandidates() &&
	mGlobalConfig.HasSynthesisFrameSize() &&
	mGlobalConfig.HasSynthesisWindowType() &&
	mGlobalConfig.HasSynthesisPhaseManagementType())
	{	
		mHaveConfig = true;
		InitConfigs();
	}
	DestroyWaitMessage();

}

void SMSBase::StoreConfig(const std::string& inputFileName)
{
	CLAMGUI::WaitMessage *wm;
	wm = CreateWaitMessage("Storing configuration xml file, please wait.");
	//Loading configuration
	XMLStorage x;
	x.UseIndentation(true);
	x.Dump(mGlobalConfig,"SMSAnalysisSynthesisConfig",inputFileName);

	delete wm;
}


bool SMSBase::LoadAnalysis(const char* fileName)
{
	mCurrentWaitMessage = CreateWaitMessage("Loading analysis data, please wait");

	mSerialization.DoSerialization( mSerialization.Load, mOriginalSegment, fileName );

	DestroyWaitMessage();
	mHaveTransformation=false;

	return true;
}


void SMSBase::StoreAnalysis(const char* fileName)
{
	mCurrentWaitMessage = CreateWaitMessage("Storing analysis data, please wait");

	mSerialization.DoSerialization( mSerialization.Store, mOriginalSegment, fileName );	

	DestroyWaitMessage();
}


bool SMSBase::LoadInputSound(void)
{
	//The File In PO
	AudioFileIn myAudioFileIn;
	AudioFileConfig infilecfg;
	infilecfg.SetFilename(mGlobalConfig.GetInputSoundFile());
	infilecfg.SetFiletype(EAudioFileType::eWave);
	if(!myAudioFileIn.Configure(infilecfg))
	{
		mHaveAudioIn = false;
		return mHaveAudioIn;
	}
			
	/////////////////////////////////////////////////////////////////////////////
	// Initialization of the processing data objects :
	TSize fileSize=myAudioFileIn.Size();

	SetSamplingRate(myAudioFileIn.SampleRate());
	
	// Spectral Segment that will actually hold data
	float duration=fileSize/mSamplingRate;
	mOriginalSegment.SetEndTime(duration);
	mOriginalSegment.SetSamplingRate(mSamplingRate);
	mOriginalSegment.mCurrentFrameIndex=0;
	mOriginalSegment.GetAudio().SetSize(fileSize);
	mOriginalSegment.GetAudio().SetSampleRate(mSamplingRate);
	

	//Read Audio File
	myAudioFileIn.Start();
	myAudioFileIn.Do(mOriginalSegment.GetAudio());
	myAudioFileIn.Stop();

	mHaveAudioIn = true;

	//TODO: this should be called from elsewhere and both methods should be refactored to reduce duplication
	LoadMorphSound();

	return mHaveAudioIn;
}

bool SMSBase::LoadMorphSound(void)
{
	//The File In PO
	AudioFileIn myAudioFileIn;
	AudioFileConfig infilecfg;
	infilecfg.SetFilename(mGlobalConfig.GetMorphSoundFile());
	infilecfg.SetFiletype(EAudioFileType::eWave);
	if(!myAudioFileIn.Configure(infilecfg))
	{
		mHaveAudioMorph = false;
		return mHaveAudioMorph;
	}
			
	/////////////////////////////////////////////////////////////////////////////
	// Initialization of the processing data objects :
	TSize fileSize=myAudioFileIn.Size();

	// Spectral Segment that will hold data to morph
	float duration=fileSize/mSamplingRate;
	mMorphSegment.SetEndTime(duration);
	mMorphSegment.SetSamplingRate(myAudioFileIn.SampleRate());
	mMorphSegment.mCurrentFrameIndex=0;
	mMorphSegment.GetAudio().SetSize(fileSize);
	mMorphSegment.GetAudio().SetSampleRate(mSamplingRate);
	

	//Read Audio File
	myAudioFileIn.Start();
	myAudioFileIn.Do(mMorphSegment.GetAudio());
	myAudioFileIn.Stop();

	mHaveAudioMorph = true;

	return mHaveAudioMorph;
}


void SMSBase::Flush(Segment& seg)
{
	seg.SetFramesArray(List<Frame>());
	seg.SetChildren(List<Segment>());
	seg.mCurrentFrameIndex=0;
	
}

void SMSBase::AnalysisProcessing()
{

	TSize size = mOriginalSegment.GetAudio().GetSize();
	
	SMSAnalysis myAnalysis(mAnalConfig);

	Flush(mOriginalSegment);
	
	/////////////////////////////////////////////////////////////////////////////
	// The main analysis processing loop.
	int k=0;
	int step=mAnalConfig.GetHopSize();
	int initialOffset=mAnalConfig.GetInitialOffset();	

	myAnalysis.Start();

	while(myAnalysis.Do(mOriginalSegment))
	{      
		k=step*(mOriginalSegment.mCurrentFrameIndex+1);
		mCurrentProgressIndicator->Update(float(k));
	}

 	myAnalysis.Stop();
}

void SMSBase::MorphAnalysisProcessing()
{

	TSize size = mMorphSegment.GetAudio().GetSize();
	
	SMSAnalysis myAnalysis(mAnalConfig);

	Flush(mMorphSegment);
	
	/////////////////////////////////////////////////////////////////////////////
	// The main analysis processing loop.
	int k=0;
	int step=mAnalConfig.GetHopSize();
	int initialOffset=mAnalConfig.GetInitialOffset();	

	myAnalysis.Start();

	while(myAnalysis.Do(mMorphSegment))
	{      
		k=step*(mMorphSegment.mCurrentFrameIndex+1);
		mCurrentProgressIndicator->Update(float(k));
	}

 	myAnalysis.Stop();


}

void SMSBase::TracksCleanupProcessing()
{
	CleanTracksConfig clcfg;
	clcfg.SetSamplingRate(mSamplingRate);
	clcfg.SetSpecSize((mGlobalConfig.GetAnalysisWindowSize()-1)/2+1);
	CleanTracks myCleanTracks;
	myCleanTracks.Configure(clcfg);
	myCleanTracks.Start();
	myCleanTracks.Do(mOriginalSegment);
	myCleanTracks.Stop();	

}

void SMSBase::MorphTracksCleanupProcessing()
{
	CleanTracksConfig clcfg;
	clcfg.SetSamplingRate(mSamplingRate);
	clcfg.SetSpecSize((mGlobalConfig.GetAnalysisWindowSize()-1)/2+1);
	CleanTracks myCleanTracks;
	myCleanTracks.Configure(clcfg);
	myCleanTracks.Start();
	myCleanTracks.Do(mMorphSegment);
	myCleanTracks.Stop();	

}

void SMSBase::DoAnalysis()
{
	AnalysisProcessing();
}

void SMSBase::DoMorphAnalysis()
{
	MorphAnalysisProcessing();
}

void SMSBase::DoTransformation()
{
	TransformProcessing();
}

void SMSBase::DoTracksCleanup()
{
	TracksCleanupProcessing();
}

void SMSBase::DoMorphTracksCleanup()
{
	MorphTracksCleanupProcessing();
}

void SMSBase::Analyze(void)
{
	TSize size = mOriginalSegment.GetAudio().GetSize();
	mCurrentProgressIndicator = CreateProgress("Analysis Processing",0,float(size));
	DoAnalysis();
	DestroyProgressIndicator();
	/*Now we will clean Tracks (TODO:This should be done on a frame by frame basis
	and included in SMSAnalysis*/
	if ( HasToDoTracksCleaning() )
		{	
			mCurrentWaitMessage = CreateWaitMessage("Cleaning tracks, please wait");

			DoTracksCleanup();

			DestroyWaitMessage();

		}
	mHaveAnalysis = true;
	mHaveSpectrum = true;
	mHaveTransformation = false;
	if(mHaveAudioMorph)
	{
		TSize size = mMorphSegment.GetAudio().GetSize();
		mCurrentProgressIndicator = CreateProgress("Morph Analysis Processing",0,float(size));
		DoMorphAnalysis();
		DestroyProgressIndicator();
		/*Now we will clean Tracks (TODO:This should be done on a frame by frame basis
		and included in SMSAnalysis*/
		if ( HasToDoTracksCleaning() )
		{	
			mCurrentWaitMessage = CreateWaitMessage("Cleaning tracks, please wait");

			DoMorphTracksCleanup();

			DestroyWaitMessage();

		}
		std::string tempSdifFilename = mGlobalConfig.GetMorphSoundFile();
		tempSdifFilename += "_tmp.sdif";
		mSerialization.DoSerialization( mSerialization.Store, mMorphSegment, tempSdifFilename.c_str() );
	}
}

void SMSBase::StoreOutputSound(void)
{
	AudioFileOut myAudioFileOut;
	AudioFileConfig outfilecfg;
	outfilecfg.SetChannels(1);
	outfilecfg.SetName("FileOut");
	outfilecfg.SetFiletype(EAudioFileType::eWave);
	outfilecfg.SetFilename(mGlobalConfig.GetOutputSoundFile());
	outfilecfg.SetSampleRate(mSamplingRate);

	myAudioFileOut.Configure(outfilecfg);

	myAudioFileOut.Start();
	myAudioFileOut.Do(mAudioOut);
	myAudioFileOut.Stop();
}


void SMSBase::StoreOutputSoundSinusoidal(void)
{
	AudioFileOut myAudioFileOut;
	AudioFileConfig outfilecfg;
	outfilecfg.SetChannels(1);
	outfilecfg.SetName("FileOut");
	outfilecfg.SetFiletype(EAudioFileType::eWave);
	std::string filename(
		mGlobalConfig.GetOutputSoundFile().
			substr(0,mGlobalConfig.GetOutputSoundFile().length()-4));
	filename += "_sin.wav";
	outfilecfg.SetFilename(filename);
	outfilecfg.SetSampleRate(mSamplingRate);
	
	myAudioFileOut.Configure(outfilecfg);

	myAudioFileOut.Start();
	myAudioFileOut.Do(mAudioOutSin);
	myAudioFileOut.Stop();
}

void SMSBase::StoreOutputSoundResidual(void)
{
	AudioFileOut myAudioFileOut;
	AudioFileConfig outfilecfg;
	outfilecfg.SetChannels(1);
	outfilecfg.SetName("FileOut");
	outfilecfg.SetFiletype(EAudioFileType::eWave);
	outfilecfg.SetSampleRate(mSamplingRate);
	std::string filename(
		mGlobalConfig.GetOutputSoundFile().
			substr(0,mGlobalConfig.GetOutputSoundFile().length()-4));
	filename += "_res.wav";
	
	outfilecfg.SetFilename(filename);
	
	myAudioFileOut.Configure(outfilecfg);

	myAudioFileOut.Start();
	myAudioFileOut.Do(mAudioOutRes);
	myAudioFileOut.Stop( );
}

void SMSBase::DoSynthesis()
{
	SynthesisProcessing();
}

void SMSBase::SynthesisProcessing()
{
	//The output Audio 
	TSize size=TSize((mTransformedSegment.GetEndTime()-
		mTransformedSegment.GetBeginTime())*mTransformedSegment.GetSamplingRate());
	mAudioOutSin.SetSize(size);
	mAudioOutRes.SetSize(size);
	mAudioOut.SetSize(size);

	//The system that contains all synthesis PO
	
	SMSSynthesis mySynthesis(mSynthConfig);
	mySynthesis.Start();
	/////////////////////////////////////////////////////////////////////////////
	// The main synthesis processing loop.
	
	Audio tmpAudioFrame,tmpAudioFrame2;
	tmpAudioFrame.SetSize(mSynthConfig.GetFrameSize());
		
	int nSynthFrames=mTransformedSegment.GetnFrames();
	int i;


	TSize synthFrameSize=mSynthConfig.GetFrameSize();
	TIndex beginIndex=-synthFrameSize/2;
	
	mTransformedSegment.mCurrentFrameIndex=0;
	for(i=0;i<nSynthFrames;i++){
		
		if(mySynthesis.Do(mTransformedSegment))
		{
			mAudioOutSin.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetSinusoidalAudioFrame());
			mAudioOutRes.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetResidualAudioFrame());
			mAudioOut.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetSynthAudioFrame());
			beginIndex+=synthFrameSize;
			mCurrentProgressIndicator->Update(float(i));
		}
		//else it is an analysis frame with negative center time and thus should not be used
	}


	mHaveAudioOut = true;

	mySynthesis.Stop();

}
void SMSBase::CopySegmentExceptAudio(const Segment& src, Segment& dest)
{
	dest=src;
	dest.RemoveAudio();
	dest.UpdateData();
	dest.CopyInit(src);
	dest.mCurrentFrameIndex=0;
}

void SMSBase::Synthesize(void)
{
	if(!mHaveTransformation)
	{
		CopySegmentExceptAudio(mOriginalSegment,mTransformedSegment);
		mHaveTransformation=true;
	}

	TSize size=TSize((mTransformedSegment.GetEndTime()-
		mTransformedSegment.GetBeginTime())*mTransformedSegment.GetSamplingRate());
	int nSynthFrames=size/mSynthConfig.GetFrameSize();

	mCurrentProgressIndicator = CreateProgress("Synthesis Processing",0,float(nSynthFrames));

	DoSynthesis();

	DestroyProgressIndicator();	

}

void SMSBase::AnalyzeMelody(void)
{
	
/* This function is just an example of the kind of things you are able to do departing from this
Analysis Synthesis applcation. The algorithm and the result are by no mean supposed to be state-of-the-art
in metadata extraction from an input sound.*/	
	
	
	ComputeLowLevelDescriptors();
	
	TData frequencies[85]={(TData)32.703, (TData)34.648, (TData)36.708, (TData)38.891, 
		(TData)41.203, (TData)43.654, (TData)46.249, (TData)48.999, (TData)51.913, (TData)55.000, (TData)58.270, (TData)61.735,
		(TData)65.406, (TData)69.296, (TData)73.416, (TData)77.782, (TData)82.407, (TData)87.307, (TData)92.499, (TData)97.999, 
		(TData)103.83, (TData)110.00, (TData)116.00, (TData)123.47, (TData)130.81, (TData)138.59, (TData)146.83, (TData)155.56, 
		(TData)164.81, (TData)174.61, (TData)185.00, (TData)196.00, (TData)207.65, (TData)220.00, (TData)233.08, (TData)246.94,
		(TData)261.63, (TData)277.18, (TData)293.66, (TData)311.13, (TData)329.63, (TData)349.23, (TData)369.99, (TData)392.00, 
		(TData)415.30, (TData)440.00, (TData)466.16, (TData)493.88, (TData)523.25, (TData)554.37, (TData)587.33, (TData)622.25, 
		(TData)659.26, (TData)698.46, (TData)739.99, (TData)783.99, (TData)830.61, (TData)880.00, (TData)932.33, (TData)987.77,
		(TData)1046.5, (TData)1108.7, (TData)1174.7, (TData)1244.5, (TData)1318.5, (TData)1396.9, (TData)1480.0, (TData)1568.0, 
		(TData)1661.2, (TData)1760.0, (TData)1864.7, (TData)1975.5,	(TData)2093.0, (TData)2217.5, (TData)2349.3, (TData)2489.0, 
		(TData)2637.0, (TData)2793.8, (TData)2960.0, (TData)3136.0, (TData)3322.4, (TData)3520.0, (TData)3729.3, (TData)3951.1,
		(TData)4186.0};

	TData analysisFrameSize, smoothFiltSize, bandThreshold, minPeakDist, globalThreshold, difSize;

	Array<PitchNote> pitch(85); 
	pitch.SetSize(85);
	int k,l;
	for(l=0; l<7;l++) {
		for(k=0; k<12; k++){
			pitch[12*l+k].SetPitch((EPitch)k);
			pitch[12*l+k].SetOctave(l+1);
		}
	}
	
	
	TData ePercentil, eThr, fPercentil, fThr, minLength;
	bool useDefault=true;
	if(useDefault) {
		ePercentil = 50;
		eThr = TData(0.0016);//0.0032;
		fPercentil = 3;
		fThr = 0;
		minLength = 2;
	}
/*old segmentator*/
	if(useDefault) {
		// Default Parameters
		analysisFrameSize = 256;	  
		smoothFiltSize = 10;
		bandThreshold = 0.15f;
		minPeakDist = 10;
		globalThreshold = 3;
		difSize = 5;

	}

	int i;

	//Configuring Segmentator to work only with fundamental and energy
	SegmentatorConfig sgConfig;
	TDescriptorsParams tmpParams;
	tmpParams.id=SpectralEnergyId;
	tmpParams.percentil=ePercentil;
	tmpParams.threshold=eThr;
	sgConfig.AddDescParams(tmpParams);
	tmpParams.id=FundamentalId;
	tmpParams.percentil=fPercentil;
	tmpParams.threshold=fThr;
	sgConfig.AddDescParams(tmpParams);
	sgConfig.SetMinSegmentLength(int(minLength));
	Segmentator mySegmentator(sgConfig);
	mySegmentator.Start();

	//Segmentate
	mySegmentator.Do(mOriginalSegment,mOriginalSegmentDescriptors);


	////////////////
	//Segmentation//
	////////////////
			
	List<Note> array;
	Array<TData> fund;
	Array<TData> energy;

	/***************************************/
	/*****Compute Melody Description********/
	/***************************************/
	int nFrames=mOriginalSegment.GetnFrames();
	for(i=0; i<nFrames; i++)
	{
		fund.AddElem(mOriginalSegment.GetFrame(i).GetFundamental().GetFreq());
		if(mOriginalSegmentDescriptors.GetFrameD(i).GetSpectrumD().GetEnergy()>eThr)
			energy.AddElem(mOriginalSegmentDescriptors.GetFrameD(i).GetSpectrumD().GetEnergy());
		else
			energy.AddElem(0);
	}
	TData ff,aux,number =0,noteEnergy=0,lastFF=0,lastEnergy=0;
	int nChildren=mOriginalSegment.GetChildren().Size();
	for(i=0;i<nChildren;i++)
	{
		ff=0; aux=0; number=0,noteEnergy=0; 
		mOriginalSegment.GetChildren()[i].SetHoldsData(false);
		MediaTime time;
		time.SetBegin(mOriginalSegment.GetChildren()[i].GetBeginTime());
		time.SetEnd(mOriginalSegment.GetChildren()[i].GetEndTime());
	
		Note myNote;
		myNote.AddPitchNote();
		myNote.UpdateData();

		// Compute Fundamental frequency mean
		TIndex b=Round(2*mOriginalSegment.GetChildren()[i].GetBeginTime()*mSamplingRate/mGlobalConfig.GetAnalysisWindowSize());
		TIndex e;
		if(mOriginalSegment.GetChildren()[i].GetEndTime()<mOriginalSegment.GetEndTime())
			e=Round(2*mOriginalSegment.GetChildren()[i].GetEndTime()*mSamplingRate/mGlobalConfig.GetAnalysisWindowSize());	
		else
			e=Round(2*mOriginalSegment.GetEndTime()*mSamplingRate/mGlobalConfig.GetAnalysisWindowSize());	

		int j;
		// Compute mean
		int count=0;
		TData offsetTh=10;
		bool onset=false;
		for(j=b; j<e; j++)
		{
			if(fund[j]>100&&fund[j]<1000)
			{
				if(j<e-3){
				if(Abs(fund[j]-fund[j+1])<offsetTh&&Abs(fund[j]-fund[j+2])<offsetTh)//offset detected
				{
					aux+=fund[j];
					count++;
					if(!onset)
					{
						onset=true;
						b=j;
					}
				}}
			}
			if(j<e-3){
			if(Abs(fund[j]-fund[j+1])>offsetTh&&Abs(fund[j]-fund[j+2])>offsetTh)//offset detected
			{
				if(onset){
				e=j;
				time.SetEnd(TData(.5*j*mGlobalConfig.GetAnalysisWindowSize()/mSamplingRate));
				break;}
			}}
		}
		aux/=count;
		// Not use the values >2*mean o<2*mean to compute the mean
		if((e-b)<=0) break;
		Audio testEnergyAudio;
		testEnergyAudio.SetSize(e-b);
		DataArray& testEnergy=testEnergyAudio.GetBuffer();
		TData maxEnergy=0;
		for(j=b; j<e; j++) {
			noteEnergy+=energy[j];
			if(energy[j]>maxEnergy) maxEnergy=energy[j];
			testEnergy.AddElem(energy[j]);
			if((fund[j] < (aux+100))&&(fund[j] > (MAX(0,(aux-100))))&&fund[j]>100&&fund[j]<1000) { 
				ff+=fund[j];
				number++;
			}
		}
		if (number)
			ff/=number;
		else ff=0;
		noteEnergy/=(e-b);
		

		if(ff<20) ff=lastFF;//make sure no false fundamental
		if(ff>800) ff/=2; //get rid of octave errors
		if(noteEnergy==0) noteEnergy=lastEnergy;
		lastFF=ff;
		lastEnergy=noteEnergy;
		myNote.SetFundFreq(ff);
		myNote.SetEnergy(noteEnergy);
		myNote.SetTime(time);
		TData min = 10000;
		TIndex m=0,n=0;
		for(m=0; m<84; m++)
			if(fabsf(frequencies[m]-ff) < min) {
				n = m;
				min = fabsf(frequencies[m]-ff);
			}
		myNote.SetPitchNote(pitch[n]);		
		if(noteEnergy>0.01&&((myNote.GetTime().GetEnd()-myNote.GetTime().GetBegin())>0.01))
		{
			if((myNote.GetTime().GetEnd()-myNote.GetTime().GetBegin())<0.2)
				myNote.GetTime().SetEnd(myNote.GetTime().GetBegin()+TData(0.2));
			array.AddElem(myNote);
		}
	}
	
	mMelody.SetNoteArray(array);

}

void SMSBase::StoreMelody(void)
{
	std::string melodyFilename(
	mGlobalConfig.GetOutputAnalysisFile().
			substr(0,mGlobalConfig.GetOutputAnalysisFile().length()-4));
	melodyFilename += "_melody.xml";

	XMLStorage x;
	x.UseIndentation(true);
	x.Dump(mMelody,"Analyzed_Melody",melodyFilename);
}

void SMSBase::LoadTransformationScore(const std::string& inputFileName)
{
	CLAMGUI::WaitMessage *wm = CreateWaitMessage("Loading configuration xml file, please wait.");
	//Loading configuration
	XMLStorage x;
	x.Restore(mTransformationScore,inputFileName);
	delete wm;
}

void SMSBase::Transform()
{
	TSize size=TSize((mOriginalSegment.GetEndTime()-mOriginalSegment.GetBeginTime())*mOriginalSegment.GetSamplingRate());
	int nSynthFrames=size/mSynthConfig.GetFrameSize();

	mCurrentProgressIndicator = CreateProgress("SMS Transformation Processing",0,float(nSynthFrames));

	DoTransformation();

	DestroyProgressIndicator();	

}


void SMSBase::SetSMSMorphFileName()
{
	SMSTransformationChainConfig::iterator configIt;
	if(mGlobalConfig.HasMorphSoundFile())
	{
		for(configIt=mTransformationScore.ConfigList_begin();configIt!=mTransformationScore.ConfigList_end();configIt++)
		{
			//Note: we are supposing only one Morph is in the chain
			if((*configIt).GetConcreteClassName()=="SMSMorph")
			{
				SMSMorphConfig& morphCfg=dynamic_cast<SMSMorphConfig&>((*configIt).GetConcreteConfig());
				morphCfg.AddFileName();
				morphCfg.UpdateData();
				std::string tempSdifFilename = mGlobalConfig.GetMorphSoundFile();
				tempSdifFilename+="_tmp.sdif";
				morphCfg.SetFileName(tempSdifFilename);
				break;
			}
		}
	}
}

void SMSBase::TransformProcessing(void)
{
	bool def=false;
	SetSMSMorphFileName();
	mTransformation.Configure(mTransformationScore);
	CopySegmentExceptAudio(mOriginalSegment,mTransformedSegment);	
	

	mTransformation.Start();
	int i = 0;
	while(mTransformation.Do())
	{
		mCurrentProgressIndicator->Update(float(i++));
	}
	mTransformation.Stop();
	mHaveTransformation=true;


}


void SMSBase::ComputeLowLevelDescriptors()
{
	// Energy Computation, note that this should be done inside a PO, added here as example
	mOriginalSegmentDescriptors.AddFramesD();
	mOriginalSegmentDescriptors.UpdateData();
	
	int i=0;
	List<FrameDescriptors> &frameDesc=mOriginalSegmentDescriptors.GetFramesD();
	List<Frame> &frames=mOriginalSegment.GetFramesArray();
	int nFrames=mOriginalSegment.GetnFrames();
	FrameDescriptors tmpFrameD;
	tmpFrameD.AddSpectrumD();
	tmpFrameD.UpdateData();
	SpectralDescriptors tmpSpecD;
	tmpSpecD.AddEnergy();
	tmpSpecD.UpdateData();
	tmpFrameD.SetSpectrumD(tmpSpecD);

	for(i=0;i<nFrames;i++)
	{
		Spectrum &tmpSpec=frames[i].GetSpectrum();
		tmpFrameD.SetpFrame(&frames[i]);
		tmpFrameD.GetSpectrumD().SetpSpectrum(&tmpSpec);
		tmpFrameD.GetSpectrumD().SetEnergy(Energy(tmpSpec.GetMagBuffer().GetPtr(),tmpSpec.GetMagBuffer().Size()));
		frameDesc.AddElem(tmpFrameD);
	}

}

void SMSBase::SetSamplingRate(TSize samplingRate)
{
	mSamplingRate=samplingRate;
	mAnalConfig.SetSamplingRate(TData(samplingRate));
	mSynthConfig.SetSamplingRate(TData(samplingRate));

	//Initialize audios sample rate
	mAudioOut.SetSampleRate(samplingRate);
	mAudioOutRes.SetSampleRate(samplingRate);
	mAudioOutSin.SetSampleRate(samplingRate);

}
