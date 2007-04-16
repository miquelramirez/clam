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
#include "AudioFileIn.hxx"       // this header imports the CLAM::AudioFileIn Processing class interface
#include "LPC_AutoCorrelation.hxx" // imports CLAM LPC_AutoCorrelation Processing declaration
#include "LPModel.hxx"             // imports CLAM::LPModel ProcessingData declaration
#include "Audio.hxx"               // imports CLAM::Audio ProcessingData declaration
#include "Spectrum.hxx"            // imports CLAM::Spectrum ProcessingData declaration
#include "SpectrumConfig.hxx"      // imports CLAM::Spectrum auxiliar datatypes declaration
#include "WindowGenerator.hxx"     // imports CLAM::WindowGenerator Processing declaration
#include "AudioMultiplier.hxx"     // imports CLAM::AudioMultiplier Processing declaration
#include "DownSampling.hxx"
#include "Plot.hxx"
#include "SystemPlots.hxx"
#include "Thread.hxx"
#include "Err.hxx"
#include <iostream>
#include <FL/fl_file_chooser.H>  // imports FLTK functions for file choosing dialogs


// Our simple speech analysis system declaration
class SimpleSpeechAnalysisSystem
{
public: // Methods
	SimpleSpeechAnalysisSystem();
	~SimpleSpeechAnalysisSystem();
	void Run();

protected: 
	// Methods

	void LoadInputSpeechFile();
	void GenerateAnalysisWindow();
	void SetupData();
	void SetupDownsampler();

protected: // Attributes

	CLAMVM::Plot                 mSpeechPlot;
	CLAMVM::Plot                 audioPlot;
	CLAM::Audio                  mInputSignal;
	CLAM::Audio                  mAnalysisWindow;
	CLAM::LPModel                mLPModel;
	CLAM::Spectrum               mLPSpectrum;
	CLAM::LPC_AutoCorrelation    mLPC;
	CLAM::DownSampling           mDownSampler;
	CLAM::AudioMultiplier        mWindowApplier;
	CLAM::TSize                  mFrameSize;
	CLAM::TSize                  mDownSampledFrameSize;
	CLAM::WindowGenerator winGen;
	CLAM::WindowGeneratorConfig wndGenCfg;


};

// And our simple speech analysis system definition
SimpleSpeechAnalysisSystem::SimpleSpeechAnalysisSystem()
	: mSpeechPlot( "speech spectrum" ),
	audioPlot("aaa")
{
	LoadInputSpeechFile();
	SetupDownsampler();
	GenerateAnalysisWindow();
	SetupData();
	mSpeechPlot.SetLabel( "Speech spectrum" );
	mSpeechPlot.SetPosition( 100, 100 );
	mSpeechPlot.SetSize( 640, 480 );
	mSpeechPlot.SetColor( CLAMVM::Color( 127, 150, 0 ) );
	mSpeechPlot.SetYRange( -50, 20 );
	audioPlot.SetLabel( "Input speech" );
	audioPlot.SetPosition( 100, 100 );
	audioPlot.SetSize( 640, 480 );
	audioPlot.SetColor( CLAMVM::Color( 127, 150, 0 ) );
	audioPlot.SetYRange( -1, 1 );


}

SimpleSpeechAnalysisSystem::~SimpleSpeechAnalysisSystem()
{
}

void SimpleSpeechAnalysisSystem::SetupDownsampler()
{
	CLAM::DownSamplingConfig cfg;
	cfg.SetDownSamplingRate( 10000 );
	mDownSampledFrameSize = CLAM::TSize( 10000.0*0.02 );
	mDownSampler.Configure( cfg );
}

void SimpleSpeechAnalysisSystem::LoadInputSpeechFile()
{
	CLAM::AudioFileConfig fileLoaderConfig;
	CLAM::AudioFileIn   fileLoader; 
	const char* filename = fl_file_chooser( "Please, select a .wav file", "*.wav", NULL );

	if ( filename == NULL )
	{
		std::cout << "User cancelled" << std::endl;
		exit(0);
	}

	fileLoaderConfig.SetFilename( filename );
	fileLoaderConfig.SetFiletype( CLAM::EAudioFileType::eWave ); 

	if ( !fileLoader.Configure( fileLoaderConfig ) )
	{
		std::cerr << "ERROR: could not open " << filename << std::endl;
		exit(-1);
	}

	CLAM::TSize nSamples = fileLoader.Size();
	CLAM::TData sampleRate = fileLoader.SampleRate();
		
	mInputSignal.SetSize( nSamples );
	mInputSignal.SetSampleRate( sampleRate );

	fileLoader.Start();      
	fileLoader.Do( mInputSignal );  
	fileLoader.Stop();

	mFrameSize = CLAM::TSize( mInputSignal.GetSampleRate()*0.02 );

}

void SimpleSpeechAnalysisSystem::GenerateAnalysisWindow()
{
	mAnalysisWindow.SetSize( mDownSampledFrameSize );
	wndGenCfg.SetType( CLAM::EWindowType::eHamming);//eBlackmanHarris92);
	wndGenCfg.SetSize( mDownSampledFrameSize );
	wndGenCfg.SetNormalize( CLAM::EWindowNormalize::eAnalysis );

}

void SimpleSpeechAnalysisSystem::SetupData()
{
	
	const unsigned lpcOrder = 22;
	mLPModel.UpdateModelOrder( lpcOrder );
	mLPModel.SetSpectralRange( mInputSignal.GetSampleRate() / 2 );
	CLAM::LPCConfig cfg;
	cfg.SetOrder( lpcOrder );
	mLPC.Configure( cfg );
	CLAM::SpecTypeFlags flags;
	flags.bMagPhase=1;
	flags.bComplex = 0;
	
	mLPSpectrum.SetSize( 100 );
	mLPSpectrum.SetSpectralRange( 5000.0 );
	mLPSpectrum.SetType( flags );
	
}

void SimpleSpeechAnalysisSystem::Run()
{

	// 20 ms length analysis frame
	CLAM::TSize hopSize = mFrameSize / 2;
	CLAM::Audio currentSampleFrame;
	currentSampleFrame.SetSize( mFrameSize );
	CLAM::Audio analysisFrame;
	analysisFrame.SetSize(  mDownSampledFrameSize );
	CLAM::TIndex currentOffset = 0;
	mLPModel.SetSpectralRange( mInputSignal.GetSampleRate()/2 );

	mDownSampler.Start();
	mWindowApplier.Start();
	mLPC.Start();

	int i = 0;

	while ( currentOffset < mInputSignal.GetSize() - hopSize )
	{
		std::cout << ".";
		std::cout.flush();
		mInputSignal.GetAudioChunk( currentOffset, currentOffset+mFrameSize,
									currentSampleFrame, true );
		mDownSampler.Do( currentSampleFrame, analysisFrame );

		audioPlot.SetData( analysisFrame.GetBuffer(), 0, 1 );

		mAnalysisWindow.SetSize( analysisFrame.GetSize() );
		wndGenCfg.SetSize( analysisFrame.GetSize() );
		winGen.Configure( wndGenCfg );
		winGen.Start();
		winGen.Do( mAnalysisWindow );
		winGen.Stop();

		mWindowApplier.Do( analysisFrame, mAnalysisWindow, analysisFrame );
		mLPC.Do( analysisFrame, mLPModel );
		mLPModel.ToSpectrum( mLPSpectrum );
		mLPSpectrum.ToDB();
		mSpeechPlot.SetData( mLPSpectrum.GetMagBuffer(), 0, 5000 );

		CLAMVM::SystemPlots::DisplayAll();

		i++;
		for ( int i = 0; i < 22; i++ )
		{
			mLPModel.GetFilterCoefficients()[i] = 0;
			mLPModel.GetReflectionCoefficients()[i] = 0;

		}
		mLPModel.SetAvgSqrFilterError( 0 );
		currentOffset += hopSize;
	}

	mLPC.Stop();
	mWindowApplier.Stop();
	mDownSampler.Stop();
}

int main( int argc, char** argv )
{
	try // Note the exception handling
	{
		CLAM::Thread               thread;
		SimpleSpeechAnalysisSystem sys;


		thread.SetThreadCode( 
			makeMemberFunctor0( sys, SimpleSpeechAnalysisSystem, Run ) );
		
		thread.Start();

		thread.Stop();

	}
	catch( CLAM::Err& e ) // Here we handle CLAM exceptions
	{
		e.Print();
		exit(-1);
	}
	catch( std::exception& e ) // Here we handle standard library exceptions
	{
		std::cerr << e.what() << std::endl;
		exit(-1);
	}

	return 0;
}

