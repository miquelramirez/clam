/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "AudioDatabaseReader.hxx"
#include <iostream>
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, AudioDatabaseReader> regAudioDatabaseReader("AudioDatabaseReader");
}

AudioDatabaseReader::AudioDatabaseReader()
{
	Configure(AudioDatabaseReaderConfig());
};

AudioDatabaseReader::AudioDatabaseReader(const AudioDatabaseReaderConfig &c)
{ 
	myMonoAudioFileReaderConfig = new MonoAudioFileReaderConfig();
	myMonoAudioFileReaderConfig->SetSourceFile( c.GetFilename() );
	//myMonoAudioFileReaderConfig->SetLoop( true );
	myMonoAudioFileReader = new MonoAudioFileReader( *myMonoAudioFileReaderConfig );
		
		// SMS Analysis configuration 
		static CLAM::SMSAnalysisConfig analConfig;	
		analConfig.SetSinWindowSize( 2049 );
		analConfig.SetHopSize(256);
		analConfig.SetSinZeroPadding(2);
		analConfig.SetResWindowSize( 1025 );

	mySMSAnalysisCore = new SMSAnalysisCore( analConfig );

	ConnectAndPublishPorts();
	
	Configure(c);
};

	// helper methods for the network tests
	const CLAM::SMSAnalysisConfig& helperAnalysisConfigInstance()
	{

		int analHopSize = 256;

//		int synthFrameSize = analHopSize;
		int analZeroPaddingFactor= 2;
		
		// SMS Analysis configuration 
		static CLAM::SMSAnalysisConfig analConfig;
		
		analConfig.SetSinWindowSize( 2049 );
		analConfig.SetHopSize(analHopSize);
//		analConfig.SetSinWindowType(mGlobalConfig.GetAnalysisWindowType());
		analConfig.SetSinZeroPadding(analZeroPaddingFactor);
		analConfig.SetResWindowSize( 1025 );
//		analConfig.SetResWindowType(mGlobalConfig.GetResAnalysisWindowType());

//		analConfig.GetPeakDetect().SetMagThreshold(mGlobalConfig.GetAnalysisPeakDetectMagThreshold());
//		analConfig.GetPeakDetect().SetMaxFreq(mGlobalConfig.GetAnalysisPeakDetectMaxFreq());
//		analConfig.GetSinTracking().SetIsHarmonic(mGlobalConfig.GetAnalysisHarmonic());
//		analConfig.GetFundFreqDetect().SetReferenceFundFreq(mGlobalConfig.GetAnalysisReferenceFundFreq());
//		analConfig.GetFundFreqDetect().SetLowestFundFreq(mGlobalConfig.GetAnalysisLowestFundFreq());
//		analConfig.GetFundFreqDetect().SetHighestFundFreq(mGlobalConfig.GetAnalysisHighestFundFreq());

		return analConfig;
	}


bool AudioDatabaseReader::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	//myMonoAudioFileReader->Configure(*myMonoAudioFileReaderConfig);
	//myMonoAudioFileReader->Start();
	
	return true;
}

AudioDatabaseReader::~AudioDatabaseReader()
{
	if (myMonoAudioFileReader)
	{
		delete myMonoAudioFileReader;
		myMonoAudioFileReader = 0;
	}
	
	if (myMonoAudioFileReaderConfig)
	{
		delete myMonoAudioFileReaderConfig;
		myMonoAudioFileReaderConfig = 0;
	}
}

void AudioDatabaseReader::ConnectAndPublishPorts()
{
	CLAM::ConnectPorts(*myMonoAudioFileReader, "Samples Read", *mySMSAnalysisCore, "Input Audio");
	// TODO	
	// change this. you need to declare portpublisher (i.e. mFundamental) and do
	// mFundamental.Publish( mSMSAnalysisCore.GetOutPort("Fundamental") );
	// where mFundamental is a OutPortPublisher<Fundamental>
	//
	// note: use mAttribut not myAttribut
//	GetOutPorts().Publish( &mySMSAnalysisCore->GetOutPort("Residual Branch Spectrum") );
//	GetOutPorts().Publish( &mySMSAnalysisCore->GetOutPort("Sinusoidal Branch Spectrum") );
//	GetOutPorts().Publish( &mySMSAnalysisCore->GetOutPort("Sinusoidal Peaks") );
//	GetOutPorts().Publish( &mySMSAnalysisCore->GetOutPort("Fundamental") );
//	GetOutPorts().Publish( &mySMSAnalysisCore->GetOutPort("Residual Spectrum") );
}
 
bool AudioDatabaseReader::Do(Audio& in)
{
	CLAM_ASSERT( AbleToExecute(), "This processing is not allowed to be executed!" );
	return myMonoAudioFileReader->Do(in);
}

bool AudioDatabaseReader::Do(Audio& inL,Audio& inR)
{
	CLAM_ASSERT( AbleToExecute(), "This processing is not allowed to be executed!" );
	return myMonoAudioFileReader->Do(inL); 
}
 
bool AudioDatabaseReader::Do(void)
{
	bool response = myMonoAudioFileReader->Do();
	mySMSAnalysisCore->Do();
	
	return response;
}
 
bool AudioDatabaseReader::ConcreteStart()
{ 
	myMonoAudioFileReader->Start();
	mySMSAnalysisCore->Start();
	
	return true;
}

bool AudioDatabaseReader::ConcreteStop()
{
	myMonoAudioFileReader->Stop();
	mySMSAnalysisCore->Stop();

return true;
}

}


