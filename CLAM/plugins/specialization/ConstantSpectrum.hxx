#ifndef ConstantSpectrum_hxx
#define ConstantSpectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MagPhaseSpectrum.hxx"
#include "MyFFT.hxx"
#include "Complex2MagPhaseSpectrum.hxx"
#include <iostream>
namespace CLAM
{


class ConstantSpectrum : public Processing
{ 
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);

	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(65536/2);
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
		return true; 
	}

	void FillSpectrum(MagPhaseSpectrum & outSpectrum)
	{

		MonoAudioFileReaderConfig readerConfig;
		readerConfig.SetSourceFile("Carnel_3P.wav");
		MonoAudioFileReader reader(readerConfig);
		CLAM_ASSERT(reader.IsConfigured(), "Reader Not Configured");
		const unsigned nSamples = reader.GetHeader().GetSamples();
		std::cout << "NSamples: " << nSamples << std::endl;
		
		AudioWindowingConfig windowerConfig;
		windowerConfig.SetSamplingRate(44100);
		windowerConfig.SetHopSize(mConfig.GetFrameSize()); // 0 would work, just to keep the parameter relation on a normal analysis
		windowerConfig.SetWindowSize(mConfig.GetFrameSize()+1);
		windowerConfig.SetFFTSize(mConfig.GetFrameSize()*2);
		windowerConfig.SetDoHalfWindowShift(false);
		windowerConfig.SetWindowType(EWindowType::eNone);
		AudioWindowing windower(windowerConfig);
		FFTConfig fftConfig; 
		fftConfig.SetAudioSize(mConfig.GetFrameSize()*2);
		MyFFT fft(fftConfig);
		Complex2MagPhaseSpectrum toMagPhase;

		ConnectPorts(reader,0,windower,0);
		ConnectPorts(windower,0,fft,0);
		ConnectPorts(fft,0,toMagPhase,0);
		InPort<MagPhaseSpectrum> fetcher;
		toMagPhase.GetOutPorts().GetByNumber(0).ConnectToIn(fetcher);

		reader.Start();
		windower.Start();
		fft.Start();
		toMagPhase.Start();
		do 
		{
			reader.Do();
		} while (not windower.CanConsumeAndProduce() );
		windower.Do();
		fft.Do();
		toMagPhase.Do();

		outSpectrum = fetcher.GetData();

		reader.Stop();
		windower.Stop();
		fft.Stop();
		toMagPhase.Stop();
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
