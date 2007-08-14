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
#include <CLAM/CLAM_Math.hxx>


#include "SMSBase.hxx"
#include <CLAM/SegmentDescriptors.hxx>
#include <CLAM/BasicOps.hxx>


#include <CLAM/AudioFileIn.hxx>
#include <CLAM/AudioFileOut.hxx>
#include <iostream>

#include <CLAM/Segment.hxx>
#include "SMSAnalysisSynthesisConfig.hxx"
#include <CLAM/SMSAnalysis.hxx>
#include <CLAM/SMSSynthesis.hxx>
#include <CLAM/CleanTracks.hxx>
#include <CLAM/Segmentator.hxx>
#include <CLAM/Normalization.hxx>
#include <CLAM/HeapDbg.hxx>

#include <fstream>
#include <CLAM/XMLStorage.hxx>

#include <CLAM/AudioIO.hxx>
#include <CLAM/AudioOut.hxx>
#include <CLAM/AudioManager.hxx>
#include <CLAM/SegmentSMSMorph.hxx>
#include <CLAM/SMSTimeStretchConfig.hxx>

namespace CLAM
{

SMSBase::SMSBase()
	: mCurrentProgressIndicator( NULL ), mCurrentWaitMessage( NULL )
{
	mTransformation.AttachIn( mOriginalSegment );
	mTransformation.AttachOut( mTransformedSegment );

	mpAnalysis=new SMSAnalysis;
	mpSynthesis=new SMSSynthesis;

}

void SMSBase::DestroyWaitMessage( )
{
	CLAM_DEBUG_ASSERT( 0!=mCurrentWaitMessage, "SMSBase destructor needs mCUrrentWaitMessage not null. (used by LoadConfig -Pau)");
	delete mCurrentWaitMessage;
	mCurrentWaitMessage = NULL;
}

void SMSBase::DestroyProgressIndicator( )
{
	delete mCurrentProgressIndicator;
	mCurrentProgressIndicator = NULL;
}



CLAMGUI::Progress* SMSBase::CreateProgress(const char* title, float from, float to)
{
	return new CLAMGUI::NullProgress;
}
CLAMGUI::WaitMessage* SMSBase::CreateWaitMessage(const char * title)
{
	return new CLAMGUI::NullWaitMessage();
}


SMSBase::~SMSBase(void)
{
	delete mpAnalysis;
	delete mpSynthesis;
}
void SMSBase::InitConfigs(void)
{
	if (!GetState().GetHasConfig()) return;

	/*global parameters*/
	int analWindowSize=mGlobalConfig.GetAnalysisWindowSize();
	int resAnalWindowSize=mGlobalConfig.GetResAnalysisWindowSize();


	int analHopSize;
	if(mGlobalConfig.GetAnalysisHopSize()<0)
	{
		analHopSize= (resAnalWindowSize-1)/2 ;
		mGlobalConfig.SetAnalysisHopSize( analHopSize );
	
	}
	else
		analHopSize = mGlobalConfig.GetAnalysisHopSize();
			
	
	int synthFrameSize;
	if(mGlobalConfig.GetSynthesisFrameSize()<0)
	{
		synthFrameSize = analHopSize;
		mGlobalConfig.SetSynthesisFrameSize(synthFrameSize);
	}
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
	mAnalConfig.GetPeakDetect().SetMaxFreq(mGlobalConfig.GetAnalysisPeakDetectMaxFreq());

	mAnalConfig.GetSinTracking().SetIsHarmonic(mGlobalConfig.GetAnalysisHarmonic());


	mAnalConfig.GetFundFreqDetect().SetReferenceFundFreq(mGlobalConfig.GetAnalysisReferenceFundFreq());
	mAnalConfig.GetFundFreqDetect().SetLowestFundFreq(mGlobalConfig.GetAnalysisLowestFundFreq());
	mAnalConfig.GetFundFreqDetect().SetHighestFundFreq(mGlobalConfig.GetAnalysisHighestFundFreq());
	
	//SMS Synthesis configuration
	mSynthConfig.SetAnalWindowSize(resAnalWindowSize);
	mSynthConfig.SetFrameSize(synthFrameSize);
	mSynthConfig.SetHopSize(synthFrameSize);

	//Configure child Processings
	GetAnalysis().Configure(mAnalConfig);	
	GetSynthesis().Configure(mSynthConfig);
	
}

void SMSBase::LoadConfig(const std::string& inputFileName)
{
	if(GetState().GetHasConfig())//This means we had a previous configuration
	{
		GetState().DefaultInit();
	}

	mCurrentWaitMessage = CreateWaitMessage( "Loading XML configuration file, please wait." );
	//Loading configuration
	XMLStorage::Restore(mGlobalConfig,inputFileName);
	GetState().SetHasConfig (false);
	if(HaveCompatibleConfig())
	{	
		GetState().SetHasConfig (true);
		InitConfigs();
	}
	DestroyWaitMessage();

}

bool SMSBase::HaveCompatibleConfig()
{
	return(mGlobalConfig.HasInputSoundFile() &&
	mGlobalConfig.HasAnalysisWindowSize() &&
	mGlobalConfig.HasAnalysisHopSize() &&
	mGlobalConfig.HasAnalysisWindowType() &&
	mGlobalConfig.HasResAnalysisWindowSize() &&
	mGlobalConfig.HasResAnalysisWindowType() &&
	mGlobalConfig.HasAnalysisZeroPaddingFactor() &&
	mGlobalConfig.HasAnalysisPeakDetectMagThreshold() &&
	mGlobalConfig.HasAnalysisPeakDetectMaxFreq() &&
	mGlobalConfig.HasAnalysisSinTrackingFreqDeviation() &&
	mGlobalConfig.HasAnalysisReferenceFundFreq() && 
	mGlobalConfig.HasAnalysisLowestFundFreq() && 
	mGlobalConfig.HasAnalysisHighestFundFreq() && 
	mGlobalConfig.HasSynthesisFrameSize() &&
	mGlobalConfig.HasSynthesisWindowType() &&
	mGlobalConfig.HasMorphSoundFile());
}

void SMSBase::StoreConfig(const std::string& inputFileName)
{
	CLAMGUI::WaitMessage *wm;
	wm = CreateWaitMessage("Storing configuration xml file, please wait.");
	//Loading configuration
	XMLStorage::Dump(mGlobalConfig,"SMSAnalysisSynthesisConfig",inputFileName);

	delete wm;
}


bool SMSBase::LoadAnalysis(const char* fileName)
{
	mCurrentWaitMessage = CreateWaitMessage("Loading analysis data, please wait");


	mSerialization.DoSerialization( mSerialization.Load, mOriginalSegment, fileName );

	GetState().SetHasAnalysis(true);
	GetState().SetHasTransformation(false);

	DestroyWaitMessage();


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
	GetState().SetHasAudioIn (LoadSound(mGlobalConfig.GetInputSoundFile(),mOriginalSegment));
	GetState().SetHasAudioMorph (LoadSound(mGlobalConfig.GetMorphSoundFile(),mMorphSegment));
	return GetState().GetHasAudioIn();
}


bool SMSBase::LoadSound(const std::string& filename,Segment& segment)
{
	AudioFileIn myAudioFileIn;
	AudioFileConfig infilecfg;
	infilecfg.SetFilename(filename);
	infilecfg.SetFiletype(EAudioFileType::eWave);
	if(!myAudioFileIn.Configure(infilecfg))
	{
		return false;
	}
			
	/////////////////////////////////////////////////////////////////////////////
	// Initialization of the processing data objects :
	TSize fileSize=myAudioFileIn.Size();

	SetSamplingRate(int(myAudioFileIn.SampleRate()));
	
	// Spectral Segment that will actually hold data
	float duration=fileSize/mSamplingRate;
	segment.SetEndTime(duration);
	segment.SetSamplingRate(mSamplingRate);
	segment.mCurrentFrameIndex=0;
	segment.GetAudio().SetSize(fileSize);
	segment.GetAudio().SetSampleRate(mSamplingRate);
	

	//Read Audio File
	myAudioFileIn.Start();
	myAudioFileIn.Do(segment.GetAudio());
	myAudioFileIn.Stop();
	return true;
}


void SMSBase::Flush(Segment& seg)
{
	seg.SetFramesArray(List<Frame>());
	seg.SetChildren(List<Segment>());
	seg.mCurrentFrameIndex=0;
	
}

void SMSBase::AnalysisProcessing()
{
	CLAM_ACTIVATE_FAST_ROUNDING;
	CLAM_DEBUG_ASSERT( 0 != mCurrentProgressIndicator, "SMSBase::AnalysisProcessing needs a ProgressIndicator");
		
	Flush(mOriginalSegment);
	
	/////////////////////////////////////////////////////////////////////////////
	// The main analysis processing loop.
	int k=0;
	int step=mAnalConfig.GetHopSize();
	
	GetAnalysis().Start();
	while(GetAnalysis().Do(mOriginalSegment))
	{   
		k=step*(mOriginalSegment.mCurrentFrameIndex+1);
		mCurrentProgressIndicator->Update(float(k));
	}

 	GetAnalysis().Stop();
	CLAM_DEACTIVATE_FAST_ROUNDING;
}

void SMSBase::MorphAnalysisProcessing()
{
	CLAM_ACTIVATE_FAST_ROUNDING;
	/* NOT USED: TSize size = mMorphSegment.GetAudio().GetSize(); */
	
	Flush(mMorphSegment);
	
	/////////////////////////////////////////////////////////////////////////////
	// The main analysis processing loop.
	int k=0;
	int step=mAnalConfig.GetHopSize();
	/* NOT USED: int initialOffset=mAnalConfig.GetInitialOffset(); */

	GetAnalysis().Start();

	while(GetAnalysis().Do(mMorphSegment))
	{      
		k=step*(mMorphSegment.mCurrentFrameIndex+1);
		mCurrentProgressIndicator->Update(float(k));
	}

 	GetAnalysis().Stop();
	CLAM_DEACTIVATE_FAST_ROUNDING;

}

void SMSBase::TracksCleanupProcessing()
{
	CLAM_ACTIVATE_FAST_ROUNDING;
	CleanTracksConfig clcfg;
	clcfg.SetSamplingRate(mSamplingRate);
	clcfg.SetSpecSize((mGlobalConfig.GetAnalysisWindowSize()-1)/2+1);
	clcfg.SetFreqDev( mGlobalConfig.GetAnalysisSinTrackingFreqDeviation() );
	clcfg.SetMaxDropOut( mGlobalConfig.GetCleanTracksMaxDropOut( ) );
	clcfg.SetMinLength( mGlobalConfig.GetCleanTracksMinLength() );
	CleanTracks myCleanTracks;
	myCleanTracks.Configure(clcfg);
	myCleanTracks.Start();
	myCleanTracks.Do(mOriginalSegment);
	myCleanTracks.Stop();	
	CLAM_DEACTIVATE_FAST_ROUNDING;
}

void SMSBase::MorphTracksCleanupProcessing()
{
	CLAM_ACTIVATE_FAST_ROUNDING;
	CleanTracksConfig clcfg;
	clcfg.SetSamplingRate(mSamplingRate);
	clcfg.SetSpecSize((mGlobalConfig.GetAnalysisWindowSize()-1)/2+1);
	CleanTracks myCleanTracks;
	myCleanTracks.Configure(clcfg);
	myCleanTracks.Start();
	myCleanTracks.Do(mMorphSegment);
	myCleanTracks.Stop();	
	CLAM_DEACTIVATE_FAST_ROUNDING;

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
	CLAM_ASSERT(GetState().GetHasAudioIn(), "Bad file-name in configuration" );

	CLAM_ACTIVATE_FAST_ROUNDING;
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
	GetState().SetHasAnalysis(true);
	GetState().SetHasSpectrum(true);
	GetState().SetHasTransformation (false);
	if(GetState().GetHasAudioMorph())
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
	}
	CLAM_DEACTIVATE_FAST_ROUNDING;
}

void SMSBase::StoreOutputSound(void)
{
	StoreSound(mGlobalConfig.GetOutputSoundFile(),mAudioOut);
}


void SMSBase::StoreOutputSoundSinusoidal(void)
{
	std::string filename(
		mGlobalConfig.GetOutputSoundFile().
			substr(0,mGlobalConfig.GetOutputSoundFile().length()-4));
	filename += "_sin.wav";
	StoreSound(filename,mAudioOutSin);
}

void SMSBase::StoreOutputSoundResidual(void)
{
	
	std::string filename(
		mGlobalConfig.GetOutputSoundFile().
			substr(0,mGlobalConfig.GetOutputSoundFile().length()-4));
	filename += "_res.wav";
	StoreSound(filename,mAudioOutRes);
}

void SMSBase::StoreSound(const std::string& fileName,const Audio& audio)
{
	AudioFileOut myAudioFileOut;
	AudioFileConfig outfilecfg;
	outfilecfg.SetChannels(1);
	outfilecfg.SetFiletype(EAudioFileType::eWave);
	outfilecfg.SetSampleRate(mSamplingRate);	

	outfilecfg.SetFilename(fileName);
	
	myAudioFileOut.Configure(outfilecfg);

	myAudioFileOut.Start();
	myAudioFileOut.Do(audio);
	myAudioFileOut.Stop( );

}

void SMSBase::DoSynthesis()
{
	SynthesisProcessing();
}

void SMSBase::SynthesisProcessing()
{
	CLAM_ACTIVATE_FAST_ROUNDING;
	GetSynthesis().Start();
	/////////////////////////////////////////////////////////////////////////////
	// The main synthesis processing loop.
	
	Audio tmpAudioFrame,tmpAudioFrame2;
	tmpAudioFrame.SetSize(mSynthConfig.GetFrameSize());
		
	int nSynthFrames=mTransformedSegment.GetnFrames();
	int i;


	TSize synthFrameSize=mSynthConfig.GetFrameSize();
	TIndex beginIndex=-synthFrameSize/2;
	
	TSize size=synthFrameSize*nSynthFrames;

	mAudioOutSin.SetSize(size);
	mAudioOutRes.SetSize(size);
	mAudioOut.SetSize(size);


	mTransformedSegment.mCurrentFrameIndex=0;
	
	for(i=0;i<nSynthFrames;i++)
	{
		
		if( !GetSynthesis().Do(mTransformedSegment) )
			continue; // it is an analysis frame with negative center time and thus should not be used
		
		mAudioOutSin.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetSinusoidalAudioFrame());
		mAudioOutRes.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetResidualAudioFrame());
		mAudioOut.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetSynthAudioFrame());
		beginIndex+=synthFrameSize;
		mCurrentProgressIndicator->Update(float(i));
	}

	GetState().SetHasAudioOut (true);
	GetSynthesis().Stop();
	CLAM_DEACTIVATE_FAST_ROUNDING;
}
void SMSBase::CopySegmentExceptAudio(Segment& src, Segment& dest)
{
	dest.SetBeginTime(src.GetBeginTime());
	dest.SetEndTime(src.GetEndTime());
	dest.SetSamplingRate(src.GetSamplingRate());
	dest.SetFramesArray(src.GetFramesArray());

	dest.CopyInit(src);
	dest.mCurrentFrameIndex=0;
}

void SMSBase::Synthesize(void)
{
	if(!GetState().GetHasTransformation())
	{
		CopySegmentExceptAudio(mOriginalSegment,mTransformedSegment);
//		GetState().SetHasTransformation(true);
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
	CLAM_ACTIVATE_FAST_ROUNDING;
	
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
			if((fund[j] < (aux+100))&&(fund[j] > (std::max(TData(0.0),(aux-TData(100)))))&&fund[j]>TData(100)&&fund[j]<TData(1000)) { 
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
	GetState().SetHasMelody(true);
	mMelody.SetNoteArray(array);

	CLAM_DEACTIVATE_FAST_ROUNDING;
}

void SMSBase::StoreMelody(void)
{
	std::string melodyFilename(
	mGlobalConfig.GetOutputAnalysisFile().
			substr(0,mGlobalConfig.GetOutputAnalysisFile().length()-4));
	melodyFilename += "_melody.xml";

	XMLStorage::Dump(mMelody,"Analyzed_Melody",melodyFilename);
}

void SMSBase::LoadTransformationScore(const std::string& inputFileName)
{
	CLAMGUI::WaitMessage *wm = CreateWaitMessage("Loading score xml file, please wait.");
	//Loading configuration
	XMLStorage::Restore(mTransformationScore,inputFileName);
	delete wm;
	GetState().SetHasTransformationScore(true);
}

void SMSBase::StoreTransformationScore( const std::string& outputFilename )
{
	CLAMGUI::WaitMessage* wm = CreateWaitMessage( "Storing transformation score" );
	XMLStorage::Dump( mTransformationScore, "SMS_Transformation_Score", outputFilename );
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

void SMSBase::ConfigureSegmentSMSMorph()
{
	SMSTransformationChain::iterator transIt=mTransformation.composite_begin();
	SMSTransformationChainConfig::iterator configIt;
	if(mGlobalConfig.HasMorphSoundFile())
	{
		for(configIt=mTransformationScore.ConfigList_begin();configIt!=mTransformationScore.ConfigList_end();configIt++,transIt++)
		{
			//Note: we are supposing only one Morph is in the chain
			if((*configIt).GetConcreteClassName()=="SegmentSMSMorph")
			{
				try{
					SegmentSMSMorph* tmpMorph= dynamic_cast<SegmentSMSMorph*>(*transIt);
					SegmentSMSMorphConfig& tmpMorphConfig= dynamic_cast<SegmentSMSMorphConfig&>((*configIt).GetConcreteConfig());
					tmpMorphConfig.SetSamplingRate(mMorphSegment.GetSamplingRate());
					tmpMorph->Configure(tmpMorphConfig);
					tmpMorph->SetSegmentToMorph(mMorphSegment);
				}
				catch (Err e)
				{
					e.Print();
				}
			}

		}
	}
}

void SMSBase::TransformProcessing(void)
{
	CLAM_ACTIVATE_FAST_ROUNDING;
	/* UNUSED: bool def=false; */
	UpdateDataInTimeStretch();
	mTransformation.Configure(mTransformationScore);
	ConfigureSegmentSMSMorph();
	CopySegmentExceptAudio(mOriginalSegment,mTransformedSegment);	
	

	 mTransformation.Start();
	int i = 0;
	while(mTransformation.Do())
	{
		CLAM_ASSERT( mCurrentProgressIndicator, 
				"SMSBase::TransformProcessing mCurrentProgressIndicator should't be NULL"
				" Probably you din't call Transfrom()" );
		mCurrentProgressIndicator->Update(float(i++));
	}
	mTransformation.Stop();
	GetState().SetHasTransformation(true);
	CLAM_DEACTIVATE_FAST_ROUNDING;

}

void SMSBase::UpdateDataInTimeStretch()
{
	SMSTransformationChainConfig::iterator cfg;
	for(cfg=mTransformationScore.ConfigList_begin();cfg!=mTransformationScore.ConfigList_end();cfg++)
	{
		if((*cfg).GetConcreteClassName()=="SMSTimeStretch")
		{
			SMSTimeStretchConfig& baseConfig=static_cast<SMSTimeStretchConfig&>((*cfg).GetConcreteConfig());
			SMSTimeStretchConfig concreteConfig;
			if(baseConfig.HasAmount()) 
				concreteConfig.SetAmount(baseConfig.GetAmount());
			else 
				concreteConfig.RemoveAmount();
			if(baseConfig.HasBPFAmount())
				concreteConfig.SetBPFAmount(baseConfig.GetBPFAmount());
			else
				concreteConfig.RemoveBPFAmount();
			concreteConfig.UpdateData();
			concreteConfig.SetSamplingRate(mSamplingRate);
			concreteConfig.SetHopSize(mGlobalConfig.GetAnalysisHopSize());
			(*cfg).AddConcreteConfig();
			(*cfg).SetConcreteConfig(concreteConfig);
		}	
	}
}


void SMSBase::ComputeLowLevelDescriptors()
{
	// Energy Computation, note that this should be done inside a PO, added here as example
	mOriginalSegmentDescriptors.AddFramesD();
	mOriginalSegmentDescriptors.UpdateData();
	
	int i=0;
	Array<FrameDescriptors> &frameDesc=mOriginalSegmentDescriptors.GetFramesD();
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
		Energy energyComputer;
		tmpFrameD.GetSpectrumD().SetEnergy(energyComputer(tmpSpec.GetMagBuffer()));
		frameDesc.AddElem(tmpFrameD);
	}

}

void SMSBase::SetSamplingRate(TSize samplingRate)
{
	mSamplingRate=samplingRate;
	mAnalConfig.SetSamplingRate(TData(samplingRate));
	mSynthConfig.SetSamplingRate(TData(samplingRate));

	//Configure child Processings
	GetAnalysis().Configure(mAnalConfig);
	GetSynthesis().Configure(mSynthConfig);

	//Initialize audios sample rate
	mAudioOut.SetSampleRate(samplingRate);
	mAudioOutRes.SetSampleRate(samplingRate);
	mAudioOutSin.SetSampleRate(samplingRate);

}

} //namespace CLAM

