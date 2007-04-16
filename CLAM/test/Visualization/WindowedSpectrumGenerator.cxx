#include "WindowedSpectrumGenerator.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include <iostream>

namespace CLAMTest
{

	void WindowedSpectrumGeneratorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetName( "unnamedGenerator" );
		SetFrameSize(1024);
		SetWindowType( EWindowType::eBlackmanHarris92 );
		SetSampleRate( TData(44100) );
	}

		WindowedSpectrumGenerator::WindowedSpectrumGenerator()
				: mConvertToLog( false )
		{
				Configure( WindowedSpectrumGeneratorConfig() );
		}

		WindowedSpectrumGenerator::WindowedSpectrumGenerator( const WindowedSpectrumGeneratorConfig& cfg)
				: mConvertToLog( false )
		{
				Configure( cfg );
		}

	bool WindowedSpectrumGenerator::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		CopyAsConcreteConfig(mConfig,cfg );

		mWindowedAudio.SetSize( mConfig.GetFrameSize() );
		mWindow.SetSize( mConfig.GetFrameSize() );

		// let's configure the children
		WindowGeneratorConfig   wndGenCfg;
		wndGenCfg.SetType( mConfig.GetWindowType() );
		wndGenCfg.SetSize( mConfig.GetFrameSize() );
		wndGenCfg.SetNormalize( EWindowNormalize::eAnalysis );

		mWindowGen.Configure( wndGenCfg );

		FFTConfig fftCfg;
		fftCfg.SetAudioSize( mConfig.GetFrameSize() );

		mAnalysis.Configure( fftCfg );

		return true;
	}

	bool WindowedSpectrumGenerator::Do()
	{
		std::cerr << "WindowedSpectrumGenerator::Do()"
			<< " not implemented yet!" 
			<< std::endl;

		return true;
	}
	
	bool WindowedSpectrumGenerator::Do( Audio& inAudio, Spectrum& outSpectra )
	{

			CLAM_ASSERT( mConfig.GetSampleRate() == inAudio.GetSampleRate(),
						"Configuration sample rate and input sample rate differ!" );

			SpecTypeFlags sflags;
			sflags.bMagPhase=true;
			sflags.bComplex=true;
			
			SpectrumConfig sConfig;
			sConfig.SetType( sflags );
			sConfig.SetSize( mConfig.GetFrameSize()/2 + 1 );

			outSpectra.Configure( sConfig );
			outSpectra.SetSpectralRange( mConfig.GetSampleRate()/2 );

			mWindow.SetSampleRate( mConfig.GetSampleRate() );
			mWindowedAudio.SetSampleRate( mConfig.GetSampleRate() );

			mWindowGen.Do( mWindow );
			mWindowApplier.Do( inAudio, mWindow, mWindowedAudio );
			mAnalysis.Do( mWindowedAudio, outSpectra );

			if ( mConvertToLog )
			{
					outSpectra.ToDB();
			}

			return true;

	}

	void WindowedSpectrumGenerator::SetWindowingType( EWindowType wtype )
	{
		WindowedSpectrumGeneratorConfig cfg = mConfig;
		cfg.SetWindowType( wtype );

		Stop();
		Configure(cfg);
	}

	bool WindowedSpectrumGenerator::ConcreteStart()
	{
			mWindowGen.Start();
			mWindowApplier.Start();
			mAnalysis.Start();

			return true;
	}

	bool WindowedSpectrumGenerator::ConcreteStop()
	{
			mWindowGen.Stop();
			mWindowApplier.Stop();
			mAnalysis.Stop();
			
			return true;
	}

}

