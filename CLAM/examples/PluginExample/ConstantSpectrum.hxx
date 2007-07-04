#ifndef ConstantSpectrum_hxx
#define ConstantSpectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/FFT.hxx>
#include "MagPhaseSpectrum.hxx"
#include <iostream>
namespace CLAM
{

class ConstantSpectrum : public Processing
{ 
	OutPort<MagPhaseSpectrum> mOut;
	MagPhaseSpectrum mSpectrum;
public:
	const char* GetClassName() const { return "ConstantSpectrum"; }
	ConstantSpectrum(const Config& config = Config()) 
		: mOut("My Output", this) 
	{
		Configure( config );
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
		ConnectPorts(reader, 0, fft, 0);
		InPort<Spectrum> fetcher;
		fft.GetOutPorts().GetByNumber(0).ConnectToIn(fetcher);
		reader.Start();
		fft.Start();
		reader.Do();
		fft.Do();
		reader.Stop();
		fft.Stop();
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
