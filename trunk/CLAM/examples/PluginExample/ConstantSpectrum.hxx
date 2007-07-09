#ifndef ConstantSpectrum_hxx
#define ConstantSpectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/FFT.hxx>
#include <CLAM/SpectralAnalysis.hxx>
#include "MagPhaseSpectrum.hxx"
#include <iostream>
namespace CLAM
{


class ConstantSpectrum : public Processing
{ 
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);
	    DYN_ATTRIBUTE( 1, public, bool, WindowedFFT);

	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(512);
	    };
	};

	OutPort<MagPhaseSpectrum> mOut;
	MagPhaseSpectrum mSpectrum;
	Config mConfig;
public:
	const char* GetClassName() const { return "ConstantSpectrum"; }
	ConstantSpectrum(const Config& config = Config()) 
		: mOut("My Output", this) 
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		// Use here the config to setup you object
		std::cout << "concrete configure!!!!" << std::endl;
		return true; 
	}

	void FillSpectrum(MagPhaseSpectrum & spectrum)
	{

		MonoAudioFileReaderConfig readerConfig;
		readerConfig.SetSourceFile("Carnel_3P.wav");
		MonoAudioFileReader reader(readerConfig);
		CLAM_ASSERT(reader.IsConfigured(), "Reader Not Configured");
		const unsigned nSamples = reader.GetHeader().GetSamples();
		std::cout << "NSamples: " << nSamples << std::endl;
		FFTConfig fftConfig; 
		fftConfig.SetAudioSize(65536);
		FFT fft(fftConfig);

		SpectralAnalysisConfig spectralAnalysisConfig;
		spectralAnalysisConfig.SetprHopSize( 16384 ); // 2^14
		spectralAnalysisConfig.SetprFFTSize( 65536 ); // 2^16
		spectralAnalysisConfig.SetprWindowSize( 65537 ); // 2^16+1
		SpectralAnalysis spectralAnalysis( spectralAnalysisConfig );

		Processing * analysis;
	       	if (mConfig.GetWindowedFFT())
			analysis = &spectralAnalysis;
		else
			analysis = &fft;

		ConnectPorts(reader, 0, *analysis, 0);
		InPort<Spectrum> fetcher;
		analysis->GetOutPorts().GetByNumber(0).ConnectToIn(fetcher);
		reader.Start();
		analysis->Start();
		do 
		{
			reader.Do();
		} while (not analysis->CanConsumeAndProduce() );
		analysis->Do();
		reader.Stop();
		analysis->Stop();
		const Spectrum & readSpectrum = fetcher.GetData();
		spectrum.spectralRange=readSpectrum.GetSpectralRange();
		spectrum.magnitudes.resize(readSpectrum.GetSize());
		spectrum.phases.resize(readSpectrum.GetSize());
		const TData * readMagitudes = readSpectrum.GetMagBuffer().GetPtr();
		const TData * readPhases = readSpectrum.GetPhaseBuffer().GetPtr();
		const unsigned spectrumSize = readSpectrum.GetSize();
		float magnitudeMean = 0;
		for (unsigned i = 0; i<spectrumSize; i++)
		{
			magnitudeMean += readMagitudes[i]/spectrumSize;
			spectrum.magnitudes[i]=readMagitudes[i];
			spectrum.phases[i]=readPhases[i];
		}
		std::cout << "Magnitude mean: " << magnitudeMean << std::endl;
		if (false and magnitudeMean>1e-14)
			for (unsigned i = 0; i<spectrumSize; i++)
				spectrum.magnitudes[i]/=magnitudeMean;

	}
 
	bool Do()
	{
		MagPhaseSpectrum & spectrum = mOut.GetData();
		if (!spectrum.magnitudes.size()) 
			FillSpectrum(spectrum);

		// Tell the ports this is done
		mOut.Produce();
		return true;
	}

};

}

#endif // ConstantSpectrum_hxx
