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


#include "AnalysisSynthesisExampleBase.hxx"
#include "SegmentDescriptors.hxx"
#include "BasicStatistics.hxx"

#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"
#include <iostream>
#include "CLAM_Math.hxx"

#include "Segment.hxx"
#include "SMSAnalysisSynthesisConfig.hxx"
#include "SMSAnalysis.hxx"
#include "SMSSynthesis.hxx"
#include "CleanTracks.hxx"
#include "Segmentator.hxx"
#include "OnsetDetection.hxx"
#include "Normalization.hxx"
#include "HeapDbg.hxx"

//Transformation class
#include "SMSFreqShift.hxx"

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

AnalysisSynthesisExampleBase::AnalysisSynthesisExampleBase()
	: mCurrentProgressIndicator( NULL ), mCurrentWaitMessage( NULL )
{
	mHaveConfig = false;
	mHaveAnalysis = false;
	mHaveAudioIn = false;
	mHaveAudioOut = false;
	mHaveTransformationScore = false;
	mHaveMelody = false;
	mHaveSpectrum = false;

	mpTransformation=NULL;

}

void AnalysisSynthesisExampleBase::DestroyWaitMessage( )
{
	delete mCurrentWaitMessage;
	mCurrentWaitMessage = NULL;
}

void AnalysisSynthesisExampleBase::DestroyProgressIndicator( )
{
	delete mCurrentProgressIndicator;
	mCurrentProgressIndicator = NULL;
}

AnalysisSynthesisExampleBase::~AnalysisSynthesisExampleBase(void)
{

}

void AnalysisSynthesisExampleBase::InitConfigs(void)
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
		synthFrameSize=(resAnalWindowSize-1)/2;
	else
		synthFrameSize=mGlobalConfig.GetSynthesisFrameSize();

	int samplingRate=int(mGlobalConfig.GetSamplingRate());
	int analZeroPaddingFactor=mGlobalConfig.GetAnalysisZeroPaddingFactor();
	// SMS Analysis configuration
	mAnalConfig.SetSinWindowSize(analWindowSize);
	mAnalConfig.SetSamplingRate(TData(samplingRate));
	mAnalConfig.SetHopSize(analHopSize);
	mAnalConfig.SetSinWindowType(mGlobalConfig.GetAnalysisWindowType());
	mAnalConfig.SetSinZeroPadding(analZeroPaddingFactor);
	mAnalConfig.SetResWindowSize(resAnalWindowSize);
	mAnalConfig.SetResWindowType(mGlobalConfig.GetResAnalysisWindowType());

	mAnalConfig.GetPeakDetect().SetMagThreshold(mGlobalConfig.GetAnalysisPeakDetectMagThreshold());
	
	mAnalConfig.GetSinTracking().SetnMaxSines(mGlobalConfig.GetAnalysisMaxSines());
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
	mSynthConfig.SetSamplingRate(TData(samplingRate));
	mSynthConfig.GetPhaseMan().SetType(mGlobalConfig.GetSynthesisPhaseManagementType());
}

void AnalysisSynthesisExampleBase::LoadConfig(const std::string& inputFileName)
{
	if(mHaveConfig)//This means we had a previous configuration
	{
		mHaveAnalysis = false;
		mHaveAudioIn = false;
		mHaveAudioOut = false;
		mHaveTransformationScore = false;
		mHaveMelody = false;
		mHaveSpectrum = false;
	}
	
	mCurrentWaitMessage = CreateWaitMessage( "Loading configuration xml file, please wait." );
	//Loading configuration
	XMLStorage x;
	x.Restore(mGlobalConfig,inputFileName);
	mHaveConfig = false;
	if(	
	mGlobalConfig.HasInputSoundFile() &&
	mGlobalConfig.HasOutputSoundFile() &&
	mGlobalConfig.HasOutputAnalysisFile() &&
	mGlobalConfig.HasInputAnalysisFile() &&
	mGlobalConfig.HasSamplingRate() &&
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

void AnalysisSynthesisExampleBase::StoreConfig(const std::string& inputFileName)
{
	CLAMGUI::WaitMessage *wm;
	wm = CreateWaitMessage("Storing configuration xml file, please wait.");
	//Loading configuration
	XMLStorage x;
	x.Dump(mGlobalConfig,"SMSAnalysisSynthesisConfig",inputFileName);

	delete wm;
}

void AnalysisSynthesisExampleBase::DoLoadSDIFAnalysis()
{
	LoadSDIFAnalysis();
}

void AnalysisSynthesisExampleBase::LoadSDIFAnalysis()
{
		
	mSDIFReader.Start();
	while(mSDIFReader.Do()) {}
	mSDIFReader.Stop();
	mHaveAnalysis = true;
	mHaveSpectrum = false;
}

void AnalysisSynthesisExampleBase::DoLoadXMLAnalysis()
{
	LoadXMLAnalysis();
}

void AnalysisSynthesisExampleBase::LoadXMLAnalysis()
{
	XMLStorage x;
	x.Restore(mSegment,mXMLInputFile);
	mHaveAnalysis = true;
	mHaveSpectrum = false;

}

void AnalysisSynthesisExampleBase::LoadAnalysis(const std::string& inputFileName)
{
	std::string ext=inputFileName.substr(inputFileName.length()-4,inputFileName.length());
	if(ext=="sdif")
	{
		mCurrentWaitMessage = CreateWaitMessage("Loading analysis data sdif file, please wait");

		SDIFInConfig cfg;
		cfg.SetMaxNumPeaks(100);
		cfg.SetFileName(inputFileName);
		cfg.SetEnableResidual(true);
		mSDIFReader.Configure(cfg);
		
		mSegment.AddAll();
		mSegment.UpdateData();
		mSDIFReader.Output.Attach(mSegment);
		DoLoadSDIFAnalysis();

		DestroyWaitMessage();
	}
	else if(ext==".xml")
	{
		mCurrentWaitMessage = CreateWaitMessage("Loading analysis data xml file, please wait");
		//Loading analysis

		DoLoadXMLAnalysis();

		DestroyWaitMessage();
	}
	else throw Err("AnalysisSynthesisExampleBase::LoadAnalysis:wrong extension to load");
	
}

void AnalysisSynthesisExampleBase::StoreSDIFAnalysis()
{
	int i;
	SDIFOutConfig cfg;
	cfg.SetSamplingRate(mGlobalConfig.GetSamplingRate());
	cfg.SetFileName(mGlobalConfig.GetOutputAnalysisFile());
	cfg.SetEnableResidual(true);
	SDIFOut SDIFWriter(cfg);
	int nFrames=mSegment.GetnFrames();
	SDIFWriter.Start();
	for(i=0;i<nFrames;i++)
	{
		SDIFWriter.Do(mSegment.GetFrame(i));
	}
	SDIFWriter.Stop();
	
}

void AnalysisSynthesisExampleBase::StoreXMLAnalysis()
{
		//first we have to get rid of not wanted data
	mSegment.RemoveAudio();
	mSegment.UpdateData();
	int i=0;

	int nFrames=mSegment.GetnFrames();
	for(i=0;i<nFrames;i++)
	{
	
		Frame& tmpFrame=mSegment.GetFrame(i);
		tmpFrame.RemoveAudioFrame();//windowed audio frame
		tmpFrame.RemoveSinusoidalAudioFrame();
		tmpFrame.RemoveResidualAudioFrame();
		tmpFrame.RemoveSinusoidalSpec();
		tmpFrame.RemoveSpectrum();//this could be kept for direct IFFT
		//Now we remove auxiliary data formats for residual spectrum
		SpecTypeFlags tmpFl;
		tmpFrame.GetResidualSpec().SetType(tmpFl);
		
	}

	XMLStorage x;
	x.Dump(mSegment,"Analyzed_Segment",mGlobalConfig.GetOutputAnalysisFile());

	//Now we add Spectrum back, it is needed for Melody analysis
	for(i=0;i<mSegment.GetnFrames();i++)
	{
		
		Frame& tmpFrame=mSegment.GetFrame(i);
		tmpFrame.AddSpectrum();//this could be kept for direct IFFT
		tmpFrame.UpdateData();			
	}

}

void AnalysisSynthesisExampleBase::DoStoreSDIFAnalysis()
{
	StoreSDIFAnalysis();
}

void AnalysisSynthesisExampleBase::DoStoreXMLAnalysis()
{
	StoreXMLAnalysis();
}

void AnalysisSynthesisExampleBase::StoreAnalysis(void)
{
	CLAM_ASSERT(mGlobalConfig.GetOutputAnalysisFile()!="","Not a valid file name");
	
	std::string ext=mGlobalConfig.GetOutputAnalysisFile().substr(mGlobalConfig.GetOutputAnalysisFile().length()-4,mGlobalConfig.GetOutputAnalysisFile().length());
	if(ext=="sdif")
	{
		mCurrentWaitMessage = CreateWaitMessage("Storing sdif file, please wait");
		
		DoStoreSDIFAnalysis();

		DestroyWaitMessage();
	}
	else if(ext==".xml")
	{
		mCurrentWaitMessage = CreateWaitMessage("Storing xml file, please wait");

		DoStoreXMLAnalysis();

		DestroyWaitMessage();
	}
	
}

bool AnalysisSynthesisExampleBase::LoadInputSound(void)
{
//	bool displayAudio=false; //true;

	//The File In PO
	AudioFileIn myAudioFileIn;
	AudioFileConfig infilecfg;
	infilecfg.SetName("FileIn");
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

	mAudioIn.SetSize(fileSize);

	//Read Audio File
	myAudioFileIn.Start();
	myAudioFileIn.Do(mAudioIn);
	myAudioFileIn.Stop();

	//Normalization is not needed for the time being
	/*NormalizationConfig NCfg;
	NCfg.SetType(3);
	Normalization mNorm(NCfg);

	mNorm.Do(mAudioIn);*/
	
	mHaveAudioIn = true;

	return mHaveAudioIn;
}

void AnalysisSynthesisExampleBase::AnalysisProcessing()
{

	TSize size = mAudioIn.GetSize();
	
	SMSAnalysis myAnalysis(mAnalConfig);

	mSegment.RemoveAll();
	mSegment.UpdateData();
	mSegment.DefaultInit();

	// Spectral Segment that will actually hold data
	float duration=size/mAnalConfig.GetSamplingRate();
	mSegment.SetHoldsData(true);
	mSegment.SetAudio(mAudioIn);
	mSegment.SetEndTime(duration);
	mSegment.SetSamplingRate(mAnalConfig.GetSamplingRate());
	mSegment.mCurrentFrameIndex=0;
	
	/////////////////////////////////////////////////////////////////////////////
	// The main analysis processing loop.
	int k=0;
	int step=mAnalConfig.GetHopSize();
	int initialOffset=mAnalConfig.GetInitialOffset();	

	myAnalysis.Start();

	do
	{      
		myAnalysis.Do(mSegment);
		mSegment.mCurrentFrameIndex++;
	    k+=step;
		mCurrentProgressIndicator->Update(float(k));
	}  while(k<=size-step-initialOffset);

	myAnalysis.Stop();


}

void AnalysisSynthesisExampleBase::TracksCleanupProcessing()
{
	CleanTracksConfig clcfg;
	clcfg.SetSamplingRate(mGlobalConfig.GetSamplingRate());
	clcfg.SetSpecSize((mGlobalConfig.GetAnalysisWindowSize()-1)/2+1);
	CleanTracks myCleanTracks;
	myCleanTracks.Configure(clcfg);
	myCleanTracks.Start();
	myCleanTracks.Do(mSegment);
	myCleanTracks.Stop();	

}

void AnalysisSynthesisExampleBase::DoAnalysis()
{
	AnalysisProcessing();
}

void AnalysisSynthesisExampleBase::DoTracksCleanup()
{
	TracksCleanupProcessing();
}

void AnalysisSynthesisExampleBase::Analyze(void)
{
	TSize size = mAudioIn.GetSize();

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
}

void AnalysisSynthesisExampleBase::StoreOutputSound(void)
{
	AudioFileOut myAudioFileOut;
	AudioFileConfig outfilecfg;
	outfilecfg.SetChannels(1);
	outfilecfg.SetName("FileOut");
	outfilecfg.SetFiletype(EAudioFileType::eWave);
	outfilecfg.SetFilename(mGlobalConfig.GetOutputSoundFile());
	outfilecfg.SetSampleRate(mGlobalConfig.GetSamplingRate());

	myAudioFileOut.Configure(outfilecfg);

	myAudioFileOut.Start();
	myAudioFileOut.Do(mAudioOut);
	myAudioFileOut.Stop();
}


void AnalysisSynthesisExampleBase::StoreOutputSoundSinusoidal(void)
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
	outfilecfg.SetSampleRate(mGlobalConfig.GetSamplingRate());
	
	myAudioFileOut.Configure(outfilecfg);

	myAudioFileOut.Start();
	myAudioFileOut.Do(mAudioOutSin);
	myAudioFileOut.Stop();
}

void AnalysisSynthesisExampleBase::StoreOutputSoundResidual(void)
{
	AudioFileOut myAudioFileOut;
	AudioFileConfig outfilecfg;
	outfilecfg.SetChannels(1);
	outfilecfg.SetName("FileOut");
	outfilecfg.SetFiletype(EAudioFileType::eWave);
	outfilecfg.SetSampleRate(mGlobalConfig.GetSamplingRate());
	std::string filename(
		mGlobalConfig.GetOutputSoundFile().
			substr(0,mGlobalConfig.GetOutputSoundFile().length()-4));
	filename += "_res.wav";
	
	outfilecfg.SetFilename(filename);
	
	myAudioFileOut.Configure(outfilecfg);

	myAudioFileOut.Start();
	myAudioFileOut.Do(mAudioOutRes);
}

void AnalysisSynthesisExampleBase::DoSynthesis()
{
	SynthesisProcessing();
}

void AnalysisSynthesisExampleBase::SynthesisProcessing()
{
	//The output Audio 
	TSize size=TSize((mSegment.GetEndTime()-mSegment.GetBeginTime())*mSegment.GetSamplingRate());
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
		
	int nSynthFrames=size/mSynthConfig.GetFrameSize();
	int i;


	TSize synthFrameSize=mSynthConfig.GetFrameSize();
	TIndex beginIndex=-synthFrameSize/2;
	
	mSegment.mCurrentFrameIndex=0;
	for(i=0;i<nSynthFrames;i++){
		if(mySynthesis.Do(mSegment))
		{
			mAudioOutSin.SetAudioChunk(beginIndex,mSegment.GetFramesArray()[i].GetSinusoidalAudioFrame());
			mAudioOutRes.SetAudioChunk(beginIndex,mSegment.GetFramesArray()[i].GetResidualAudioFrame());
			mAudioOut.SetAudioChunk(beginIndex,mSegment.GetFramesArray()[i].GetSynthAudioFrame());
			beginIndex+=synthFrameSize;
			mCurrentProgressIndicator->Update(float(i));
		}
		//else it is an analysis frame with negative center time and thus should not be used
	}


	mHaveAudioOut = true;

	mySynthesis.Stop();

}

void AnalysisSynthesisExampleBase::Synthesize(void)
{
	TSize size=TSize((mSegment.GetEndTime()-mSegment.GetBeginTime())*mSegment.GetSamplingRate());
	int nSynthFrames=size/mSynthConfig.GetFrameSize();

	mCurrentProgressIndicator = CreateProgress("Synthesis Processing",0,float(nSynthFrames));

	DoSynthesis();

	DestroyProgressIndicator();	

}

void AnalysisSynthesisExampleBase::AnalyzeMelody(void)
{
	
/* This function is just an example of the kind of things you are able to do departing from this
Analysis Synthesis applcation. The algorithm and the result are by no mean supposed to be state-of-the-art
in metadata extraction from an input sound.*/	
	
	
	ComputeLowLevelDescriptors();
	
	TData frequencies[85]={32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 51.913, 55.000, 58.270, 61.735,
												 65.406, 69.296, 73.416, 77.782, 82.407, 87.307, 92.499, 97.999, 103.83, 110.00, 116.00, 123.47,
												 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
												 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88,
												 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77,
												 1046.5, 1108.7, 1174.7, 1244.5, 1318.5, 1396.9, 1480.0, 1568.0, 1661.2, 1760.0, 1864.7, 1975.5,
												 2093.0, 2217.5, 2349.3, 2489.0, 2637.0, 2793.8, 2960.0, 3136.0, 3322.4, 3520.0, 3729.3, 3951.1,
												 4186.0};

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
	mySegmentator.Do(mSegment,mSegmentDescriptors);


		////////////////
	//Segmentation//
	////////////////
			
	//This new segmentator is still to be tested
/*	OnsetDetectionConfig onsetconfig;
	onsetconfig.SetFrameSize(analysisFrameSize);
	onsetconfig.SetSmoothFiltSize(smoothFiltSize);
	onsetconfig.SetBandThreshold(bandThreshold);
	onsetconfig.SetMinPeakDist(minPeakDist);
	onsetconfig.SetGlobalThreshold(globalThreshold);
	onsetconfig.SetDifSize(difSize);
	OnsetDetection onset(onsetconfig);

	//Segmentation
	onset.Start();
	onset.Do(mSegment);
*/
	List<Note> array;
	Array<TData> fund;
	Array<TData> energy;

	/***************************************/
	/*****Compute Melody Description********/
	/***************************************/
	for(i=0; i<mSegment.GetnFrames(); i++)
	{
		fund.AddElem(mSegment.GetFrame(i).GetFundamental().GetFreq());
		if(mSegmentDescriptors.GetFrameD(i).GetSpectrumD().GetEnergy()>eThr)
			energy.AddElem(mSegmentDescriptors.GetFrameD(i).GetSpectrumD().GetEnergy());
		else
			energy.AddElem(0);
	}
	TData ff,aux,number =0,noteEnergy=0,lastFF=0,lastEnergy=0;
	for(i=0;i<mSegment.GetChildren().Size();i++)
	{
		ff=0; aux=0; number=0,noteEnergy=0; 
		mSegment.GetChildren()[i].SetHoldsData(false);
		MediaTime time;
		time.SetBegin(mSegment.GetChildren()[i].GetBeginTime());
		time.SetEnd(mSegment.GetChildren()[i].GetEndTime());
	
		Note myNote;
		myNote.AddPitchNote();
		myNote.UpdateData();

		// Compute Fundamental frequency mean
		TIndex b=roundInt(2*mSegment.GetChildren()[i].GetBeginTime()*mGlobalConfig.GetSamplingRate()/mGlobalConfig.GetAnalysisWindowSize());
		TIndex e;
		if(mSegment.GetChildren()[i].GetEndTime()<mSegment.GetEndTime())
			e=roundInt(2*mSegment.GetChildren()[i].GetEndTime()*mGlobalConfig.GetSamplingRate()/mGlobalConfig.GetAnalysisWindowSize());	
		else
			e=roundInt(2*mSegment.GetEndTime()*mGlobalConfig.GetSamplingRate()/mGlobalConfig.GetAnalysisWindowSize());	

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
				time.SetEnd(TData(.5*j*mGlobalConfig.GetAnalysisWindowSize()/mGlobalConfig.GetSamplingRate()));
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

void AnalysisSynthesisExampleBase::StoreMelody(void)
{
	std::string melodyFilename(
	mGlobalConfig.GetOutputAnalysisFile().
			substr(0,mGlobalConfig.GetOutputAnalysisFile().length()-4));
	melodyFilename += "_melody.xml";

	XMLStorage x;
	x.Dump(mMelody,"Analyzed_Melody",melodyFilename);
}

void AnalysisSynthesisExampleBase::LoadTransformationScore(const std::string& inputFileName)
{
	CLAMGUI::WaitMessage *wm = CreateWaitMessage("Loading configuration xml file, please wait.");
	//Loading configuration
	XMLStorage x;
	x.Restore(mTransformationScore,inputFileName);
	delete wm;
}

void AnalysisSynthesisExampleBase::Transform(void)
{
	bool def=false;
	if(!mpTransformation)
	{
		SetTransformation(new SMSFreqShift);
		def=true;
	}
	mpTransformation->Configure(mTransformationScore);
	mpTransformation->Do(mSegment,mSegment);
	if (def) {
		delete mpTransformation;
		mpTransformation=NULL;}
}

void AnalysisSynthesisExampleBase::SetTransformation(SMSTransformation* pTransformation)
{
	mpTransformation=pTransformation;
}

void AnalysisSynthesisExampleBase::ComputeLowLevelDescriptors()
{
	// Energy Computation, note that this should be done inside a PO, added here as example
	mSegmentDescriptors.AddFramesD();
	mSegmentDescriptors.UpdateData();
	
	int i=0;
	List<FrameDescriptors> &frameDesc=mSegmentDescriptors.GetFramesD();
	List<Frame> &frames=mSegment.GetFramesArray();
	int nFrames=mSegment.GetnFrames();
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

void AnalysisSynthesisExampleBase::PlayOutputSound()
{
	if(mHaveAudioOut)
	{
		Play(mAudioOut);
	}
}

void AnalysisSynthesisExampleBase::PlayInputSound()
{
	if(mHaveAudioIn)
	{
		Play(mAudioIn);
	}
}

void AnalysisSynthesisExampleBase::PlaySinusoidal()
{
	if(mHaveAudioIn)
	{
		Play(mAudioOutSin);
	}
}

void AnalysisSynthesisExampleBase::PlayResidual()
{
	if(mHaveAudioIn)
	{
		Play(mAudioOutRes);
	}
}

void AnalysisSynthesisExampleBase::Play(const Audio& audio)
{
	
	TSize outBufferSize=512;
	AudioManager audioManager(mGlobalConfig.GetSamplingRate(),outBufferSize);
	AudioIOConfig outCfgL;
	AudioIOConfig outCfgR;

	outCfgL.SetName("left out");
	outCfgL.SetChannelID(0);
	outCfgR.SetName("right out");
	outCfgR.SetChannelID(1);

	AudioOut outputL(outCfgL);
	AudioOut outputR(outCfgR);
	
	Audio  tmpAudioBuffer;
	tmpAudioBuffer.SetSize(outBufferSize);
	TSize size=audio.GetSize();
	AudioManager::Current().Start();
	outputL.Start();
	outputR.Start();
	int i;
	for(i=0;i<size;i+=outBufferSize)
	{
		audio.GetAudioChunk(i,i+outBufferSize,tmpAudioBuffer,false);
		outputR.Do(tmpAudioBuffer);
		outputL.Do(tmpAudioBuffer);
	}
}
