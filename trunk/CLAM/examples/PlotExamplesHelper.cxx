#include "PlotExamplesHelper.hxx"
#include "Audio.hxx"
#include "FFT_rfftw.hxx"
#include "WindowGenerator.hxx"
#include "AudioMultiplier.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx" 
#include "FDFilterGen.hxx"
#include "SpectrumProduct.hxx"
#include <cstdlib>
#include <ctime>

// The following code is here just for generating some data. Go directly
// to the main() function

void generateLowPassFilter( CLAM::Spectrum& spectrum )
{
	CLAM::FDFilterGenConfig myFilterGeneratorConfig;
	myFilterGeneratorConfig.SetType(CLAM::EFDFilterType::eLowPass);
	myFilterGeneratorConfig.SetSpectralRange(22050);
	myFilterGeneratorConfig.SetGain(-12);
	myFilterGeneratorConfig.SetLowCutOff(1000);
	myFilterGeneratorConfig.SetStopBandSlope(128);
	
	CLAM::FDFilterGen myFilterGenerator;
	
	myFilterGenerator.Configure( myFilterGeneratorConfig );
	
	myFilterGenerator.Start();
	
	CLAM::SpecTypeFlags specFlags;
	
	specFlags.bMagPhase = 0;
	specFlags.bMagPhaseBPF=1;
	spectrum.SetType(specFlags);
	spectrum.SetSpectralRange(22050);
	spectrum.SetScale(CLAM::EScale::eLog);
	myFilterGenerator.Do(spectrum);
	
	myFilterGenerator.Stop();
}

void generateBandPassFilter( CLAM::Spectrum& spectrum )
{
	CLAM::FDFilterGenConfig myFilterGeneratorConfig;
	myFilterGeneratorConfig.SetType(CLAM::EFDFilterType::eBandPass);
	myFilterGeneratorConfig.SetSpectralRange(22050);
	myFilterGeneratorConfig.SetGain(1);
	myFilterGeneratorConfig.SetLowCutOff(2500);
	myFilterGeneratorConfig.SetHighCutOff( 6000 );
	myFilterGeneratorConfig.SetStopBandSlope(64);
	myFilterGeneratorConfig.SetPassBandSlope(14);
	
	CLAM::FDFilterGen myFilterGenerator;
	
	myFilterGenerator.Configure( myFilterGeneratorConfig );
	
	myFilterGenerator.Start();
	
	CLAM::SpecTypeFlags specFlags;

	specFlags.bMagPhase = 0;	
	specFlags.bMagPhaseBPF=1;
	spectrum.SetType(specFlags);
	spectrum.SetSpectralRange(22050);
	spectrum.SetScale(CLAM::EScale::eLog);
	myFilterGenerator.Do(spectrum);
	
	myFilterGenerator.Stop();
}

void generateHighPassFilter( CLAM::Spectrum& spectrum )
{
	CLAM::FDFilterGenConfig myFilterGeneratorConfig;
	myFilterGeneratorConfig.SetType(CLAM::EFDFilterType::eHighPass);
	myFilterGeneratorConfig.SetSpectralRange(22050);
	myFilterGeneratorConfig.SetGain(1);
	myFilterGeneratorConfig.SetHighCutOff( 6000 );
	myFilterGeneratorConfig.SetPassBandSlope(14);
	
	CLAM::FDFilterGen myFilterGenerator;
	
	myFilterGenerator.Configure( myFilterGeneratorConfig );
	
	myFilterGenerator.Start();
	
	CLAM::SpecTypeFlags specFlags;
	
	specFlags.bMagPhase = 0;
	specFlags.bMagPhaseBPF=1;
	spectrum.SetType(specFlags);
	spectrum.SetSpectralRange(22050);
	spectrum.SetScale(CLAM::EScale::eLog);
	myFilterGenerator.Do(spectrum);
	
	myFilterGenerator.Stop();

}

void generateSignalSpectrum( CLAM::Spectrum& spectrum )
{
	const CLAM::TSize samples=1024;      // the number of samples in the audio signal
	const CLAM::TData sampleRate=44100.0; // the sample rate of the audio signal
	const CLAM::TData sineFreq = 400.0;  // frequency of the audio signal sinusoidal
	const CLAM::TData fundFreq = 1200.0;

	CLAM::Audio myaudio;
	myaudio.SetSize(samples);
	myaudio.SetSampleRate( sampleRate );
	for ( CLAM::TSize i=0; i<samples; i++ )
		myaudio.GetBuffer()[i]=
			0.1 + 
			0.5*sin(2.0*PI*sineFreq*(((float)i)/sampleRate)) + 
			0.4*sin(2.0*PI*(sineFreq+fundFreq)*(((float)i)/sampleRate)) +
			0.3*sin(2.0*PI*(sineFreq+(2.0*fundFreq))*(((float)i)/sampleRate)) +
			0.2*sin(2.0*PI*(sineFreq+(3.0*fundFreq))*(((float)i)/sampleRate)) +
			0.1*sin(2.0*PI*(sineFreq+(4.0*fundFreq))*(((float)i)/sampleRate));

	CLAM::Audio window;
	window.SetSize( samples );

	CLAM::Audio windowedAudio;
	windowedAudio.SetSize( samples );

	CLAM::WindowGeneratorConfig wndGenCfg;
	wndGenCfg.SetType( CLAM::EWindowType::eBlackmanHarris92 );
	wndGenCfg.SetSize( samples );
	wndGenCfg.SetNormalize( CLAM::EWindowNormalize::eAnalysis );

	CLAM::WindowGenerator winGen;

	winGen.Configure( wndGenCfg );
	winGen.Start();
	winGen.Do( window );
	winGen.Stop();

	CLAM::SpecTypeFlags specFlags;
	specFlags.bComplex=0;
	specFlags.bMagPhase = 1;
	spectrum.SetType(specFlags);
	spectrum.SetSize(samples/2+1);
	spectrum.SetSpectralRange( 22050 );

	CLAM::FFTConfig fconfig;
	fconfig.SetAudioSize(samples);

	// Windowing and Fourier transform

	CLAM::AudioMultiplier windowApplier;

	windowApplier.Start(); 
	windowApplier.Do( myaudio, window, windowedAudio );
	windowApplier.Stop();

	CLAM::FFT_rfftw myfft;
	myfft.Configure( fconfig );
	myfft.Start();
	myfft.Do( windowedAudio, spectrum );

}

void filter( CLAM::Spectrum& signal, 
	     CLAM::Spectrum& filterResponse,
	     CLAM::Spectrum& filteredSignal )
{
	filterResponse.ToLinear();

	CLAM::SpecTypeFlags flags;

	signal.GetType( flags );
	
	filteredSignal.SetType( flags );
	filteredSignal.SetSize( signal.GetSize() );

	CLAM::SpectrumProduct filterApplier;
	filterApplier.SetPrototypes( signal, filterResponse, filteredSignal );
	filterApplier.Start();
	filterApplier.Do( signal, filterResponse, filteredSignal );
	filterApplier.Stop();

	signal.ToDB();
	filterResponse.ToDB();
	filteredSignal.ToDB();
}

void measureRandRandomness( CLAM::Array< CLAM::TData >& measure )
{
	srand( time( NULL ) );

	measure.Resize( 64 );
	measure.SetSize( 64 );
	
	for ( unsigned i = 0; i < 10000; i++ )
	{
		measure[ rand()%64 ]+=1.0f;
	}

	for ( int j = 0; j < measure.Size(); j++ )
	{
		measure[j] *= 1e-4;
	}
}
