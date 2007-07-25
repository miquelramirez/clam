#ifndef LowLatencyConvolution_hxx
#define LowLatencyConvolution_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MyFFT.hxx"
#include "ComplexSpectrum.hxx"
#include "ComplexSpectrumProduct.hxx"
#include "ComplexSpectrumSum.hxx"
#include <algorithm>

namespace CLAM
{

class LowLatencyConvolution : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);

	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(512);
	    };
	};

private:
	InPort<ComplexSpectrum> _input;
	OutPort<ComplexSpectrum> _output;
	Config _config;
	std::vector<ComplexSpectrum> _responseSpectrums;
	std::vector<ComplexSpectrum> _delayedSpectrums;
	unsigned _current;

public:
	const char* GetClassName() const { return "LowLatencyConvolution"; }
	LowLatencyConvolution(const Config& config = Config()) 
		: _input("Input", this)
		, _output("Output", this) 
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		MonoAudioFileReaderConfig readerConfig;
		readerConfig.SetSourceFile("Carnel_3P.wav");
		MonoAudioFileReader reader(readerConfig);
		// TODO: Configuration error instead
		CLAM_ASSERT(reader.IsConfigured(), "Reader Not Configured");
		const unsigned nSamples = reader.GetHeader().GetSamples();
		std::cout << "NSamples: " << nSamples << std::endl;

		AudioWindowingConfig windowerConfig;
		windowerConfig.SetSamplingRate(44100); // TODO: Take it from the file
		windowerConfig.SetHopSize(_config.GetFrameSize());
		windowerConfig.SetWindowSize(_config.GetFrameSize()+1);
		windowerConfig.SetFFTSize(_config.GetFrameSize()*2);
		windowerConfig.SetDoHalfWindowShift(false);
		windowerConfig.SetWindowType(EWindowType::eNone);
		AudioWindowing windower(windowerConfig);
		if (!windower.IsConfigured())
		{
			AddConfigErrorMessage("Configuring the inner AudioWindowing:\n");
			AddConfigErrorMessage(windower.GetConfigErrorMessage());
			return false;
		}
		FFTConfig fftConfig; 
		fftConfig.SetAudioSize(_config.GetFrameSize()*2);
		MyFFT fft(fftConfig);

		ConnectPorts(reader,0,windower,0);
		ConnectPorts(windower,0,fft,0);
		InPort<ComplexSpectrum> fetcher;
		fft.GetOutPorts().GetByNumber(0).ConnectToIn(fetcher);

		reader.Start();
		windower.Start();
		fft.Start();
		while (reader.Do())
		{
			windower.Do();
			fft.Do();
			_responseSpectrums.push_back(fetcher.GetData());
			fetcher.Consume();
		}
		std::cout << "LowLatencyConvolution: N blocks " << _responseSpectrums.size() << std::endl; 

		reader.Stop();
		windower.Stop();
		fft.Stop();

		_delayedSpectrums.resize(_responseSpectrums.size());
		for (unsigned i=0; i<_responseSpectrums.size(); i++)
		{
			ComplexSpectrum & spectrum = _delayedSpectrums[i];
			spectrum.spectralRange=_responseSpectrums[0].spectralRange;
			spectrum.bins.assign(_responseSpectrums[0].bins.size(),std::complex<CLAM::TData>());
		}
		_current=0;
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }
	bool Do()
	{
		const ComplexSpectrum & input = _input.GetData();
		ComplexSpectrum & output = _output.GetData();
		unsigned nBlocks = _responseSpectrums.size()/2;

		output.spectralRange = input.spectralRange;
		const unsigned nBins = input.bins.size(); 
		output.bins.resize( nBins );
		for (unsigned i=0; i<output.bins.size(); i++)
			output.bins[i]=0;
		_delayedSpectrums[_current++].bins=input.bins;
		if (_current>=nBlocks) _current=0;
		ComplexSpectrumProduct product;
		ComplexSpectrumSum sum;
		unsigned delayIndex=_current;
		for (unsigned i=0; i<nBlocks; i++)
		{
			if (delayIndex>=nBlocks) delayIndex=0;
			ComplexSpectrum productSpectrum;
			ComplexSpectrum sumSpectrum;
			product.Do(_responseSpectrums[nBlocks-i-1], _delayedSpectrums[delayIndex++], productSpectrum);
			sum.Do(productSpectrum, output, sumSpectrum);
			output.bins=sumSpectrum.bins;
		}
		// Tell the ports this is done
		_input.Consume();
		_output.Produce();
		return true;
	}

};

} // namespace CLAM

#endif // LowLatencyConvolution_hxx
