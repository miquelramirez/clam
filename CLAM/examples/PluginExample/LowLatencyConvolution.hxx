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
#include "ComplexSpectrumMixer.hxx"
#include <algorithm>
#include <cmath>

namespace CLAM
{

class LowLatencyConvolution : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);
	    DYN_ATTRIBUTE( 1, public, bool, UseExternalDataBase );
	    DYN_ATTRIBUTE( 2, public, AudioInFilename, ImpulseResponseAudioFile );

	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(512);
		  SetUseExternalDataBase(false);
	    };
	};

private:
	InPort<ComplexSpectrum> _input;
	OutPort<ComplexSpectrum> _output;
	InPort< std::vector<ComplexSpectrum>* > _impulseResponse;

	Config _config;
	std::vector<ComplexSpectrum> _responseSpectrums;
	std::vector<ComplexSpectrum> _delayedSpectrums;
	unsigned _current;
	ComplexSpectrumProduct _product;
	ComplexSpectrumSum _sum;

public:
	const char* GetClassName() const { return "LowLatencyConvolution"; }
	LowLatencyConvolution(const Config& config = Config()) 
		: _input("Input", this)
		, _output("Output", this)
		, _impulseResponse("ImpulseResponse", this)
	{
		Configure( config );
	}
	void InitialitzeDelaySpectrums( unsigned nBlocks, unsigned spectrumSize, TData spectralRange)
	{
		std::cout << "LowLatencyConvolution: N blocks " << nBlocks << std::endl; 
		_delayedSpectrums.resize(nBlocks);
		for (unsigned i=0; i<nBlocks; i++)
		{
			ComplexSpectrum & spectrum = _delayedSpectrums[i];
			spectrum.spectralRange=spectralRange;
			spectrum.bins.assign( spectrumSize, std::complex<CLAM::TData>() );
		}
		_current=0;
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }


	bool Do()
	{
		const ComplexSpectrum & input = _input.GetData();
		ComplexSpectrum & output = _output.GetData();
		std::vector<ComplexSpectrum> & impulseResponse = *_impulseResponse.GetData();
		unsigned nBlocks = std::min(impulseResponse.size(),1000u);
		if (_delayedSpectrums.size()!=nBlocks)
			InitialitzeDelaySpectrums( nBlocks, impulseResponse[0].bins.size(), impulseResponse[0].spectralRange );

		output.spectralRange = input.spectralRange;
		const unsigned nBins = input.bins.size(); 
		output.bins.resize( nBins );
		for (unsigned i=0; i<output.bins.size(); i++)
			output.bins[i]=0;
		_delayedSpectrums[_current].bins=input.bins;
		unsigned delayIndex=_current+1;
		ComplexSpectrum productSpectrum;
		for (unsigned i=0; i<nBlocks; i++)
		{
			if (delayIndex>=nBlocks) delayIndex=0;
			_product.Do(impulseResponse[nBlocks-i-1], _delayedSpectrums[delayIndex++], productSpectrum);
			_sum.Do(productSpectrum, output, output);
		}
		_current++;
		if (_current>=nBlocks) 
		{
			_current=0;
		}
		// Tell the ports this is done
		_input.Consume();
		_impulseResponse.Consume();
		_output.Produce();
		return true;
	}
private:
	bool ComputeResponseSpectrums(const std::string & wavfile, std::vector<ComplexSpectrum> & responseSpectrums)
	{
		std::cout << "---- wavfile "<< wavfile <<std::endl;

		MonoAudioFileReaderConfig readerConfig;
		readerConfig.SetSourceFile(wavfile);
		MonoAudioFileReader reader(readerConfig);
		if (!reader.IsConfigured())
		{
			AddConfigErrorMessage("Configuring the inner AudioFileReader:\n");
			AddConfigErrorMessage(reader.GetConfigErrorMessage());
			return false;
		}
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

		responseSpectrums.clear();
		reader.Start();
		windower.Start();
		fft.Start();
		
		for (bool samplesAvailable=true; samplesAvailable; )
		{
			samplesAvailable = reader.Do();
			if (!windower.CanConsumeAndProduce()) continue;
			windower.Do();
			fft.Do();
			responseSpectrums.push_back(fetcher.GetData());
//			fetcher.GetData().dump(std::cout);
			fetcher.Consume();
		}

		reader.Stop();
		windower.Stop();
		fft.Stop();
		return true;
	}

};

} // namespace CLAM

#endif // LowLatencyConvolution_hxx
