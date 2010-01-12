/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef AllPassFilterGenerator_hxx
#define AllPassFilterGenerator_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include "MyIFFT.hxx"
#include <CLAM/AudioInFilename.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <cstdlib>

namespace CLAM
{

/**
 This processing feeds a constant ImpulseResponse (DFT stream representation)
 of an audio file.
 @param FrameSize [Config] The size of the frames in samples for the DFT.
 @param AudioInFilename [Config] The name of the wave file to load.
 @param[out] ImpulseResponse [Port] The generated impulse response.
 @ingroup RealTimeConvolution

 @todo: (TODO) make it multichannel with a similar behavior as sndfileplayer (config parameter with num channels)
*/
class AllPassFilterGenerator : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 4, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 1, public, unsigned, FilterSize);
		DYN_ATTRIBUTE( 2, public, unsigned, MaxFilterSize);
		DYN_ATTRIBUTE( 3, public, unsigned, NChannels);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFrameSize(1024);
			SetFilterSize(256);
			SetMaxFilterSize(4096);
		};
	};

private:
	Config _config;
	typedef std::vector<ComplexSpectrum> ImpulseResponse;
	std::vector<ImpulseResponse> _responseSpectrums;
	std::vector< OutPort<ImpulseResponse*> *> _impulseResponseOutputsPorts;
	FloatInControl _filterSize;
	unsigned _currentFilterSize;

public:
	const char* GetClassName() const { return "AllPassFilterGenerator"; }
	AllPassFilterGenerator(const Config& config = Config()) 
		: _filterSize("FilterSize", this)
		, _currentFilterSize(0)
	{
		Configure( config );
	}
	~AllPassFilterGenerator()
	{
		ResizePorts(0);
	}
	void FillRandomPhaseSpectrum(ComplexSpectrum & spectrum)
	{
		spectrum.spectralRange = 22050;
		spectrum.bins[0] = 1.;
		spectrum.bins[spectrum.bins.size()-1]=1.;
		for (unsigned i=1; i<spectrum.bins.size()-1; i++)
		{
			float phase = 2*M_PI*std::rand()/RAND_MAX;
			spectrum.bins[i] = std::complex<float>(std::cos(phase), std::sin(phase));
		}
	}
	bool FillData(ImpulseResponse & ir)
	{
		unsigned filterSize = std::min(std::max(_currentFilterSize&~1, 2u), _config.GetMaxFilterSize());
		std::cout << filterSize << std::endl;
		OutPort<ComplexSpectrum> feeder;
		MyIFFT::Config config;
		config.SetAudioSize(filterSize);
		MyIFFT ifft(config);
		if (not ifft.IsConfigured()) return AddConfigErrorMessage(ifft.GetConfigErrorMessage());
		// TODO: Check config error
		CLAM::InPort<Audio> fetcher;
		ifft.GetOutPort(0).ConnectToIn(fetcher);
		feeder.ConnectToIn(ifft.GetInPort(0));
		// TODO: connnection could cascar

		ifft.Start();
		ComplexSpectrum & spectrum = feeder.GetData();
		spectrum.bins.resize(filterSize/2+1);
		FillRandomPhaseSpectrum(spectrum);
		feeder.Produce();
		ifft.Do();
		ifft.Stop();
		const CLAM::Audio & outputAudio = fetcher.GetData();
		TData * buffer = outputAudio.GetBuffer().GetPtr();
		std::vector<double> audio(buffer, buffer+outputAudio.GetSize());
		audio.resize(_config.GetMaxFilterSize());
		std::string errmsg;
		if (not computeResponseSpectrums(audio, ir, _config.GetFrameSize(), errmsg, 44100, 0))
			return AddConfigErrorMessage(errmsg);
		return true;
	}
	bool FillData()
	{
		for (unsigned channel=0; channel<_responseSpectrums.size(); channel++)
			if (not FillData(_responseSpectrums[channel])) return false;
		return true;
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		ResizePorts(_config.GetNChannels());
		_currentFilterSize = _config.GetFilterSize();
		_filterSize.DoControl(_currentFilterSize);
		return FillData();
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	bool Do()
	{
		unsigned oldSize = _currentFilterSize;
		_currentFilterSize = Round(_filterSize.GetLastValue());
		if (_currentFilterSize != oldSize)
		{
			if (not FillData())
			{
				std::cout << "Running error: " << GetConfigErrorMessage() << std::endl;
			}
		}
		for (unsigned i=0; i<_impulseResponseOutputsPorts.size(); i++)
		{
			CLAM::OutPort<ImpulseResponse*> & ir = *_impulseResponseOutputsPorts[i];
			ir.GetData()= &_responseSpectrums[i];
			ir.Produce();
		}
		return true;
	}

	void ResizePorts(unsigned nChannels)
	{
		unsigned oldNChannels = _impulseResponseOutputsPorts.size();
		// Delete if less
		for (unsigned port = oldNChannels; port < oldNChannels; port++)
			delete _impulseResponseOutputsPorts[port];
		// Resize the container vector
		_impulseResponseOutputsPorts.resize(nChannels);
		// Add missing
		for (unsigned speaker=oldNChannels; speaker<nChannels; speaker++)
		{
			std::ostringstream os;
			os << speaker+1;
			_impulseResponseOutputsPorts[speaker] = new OutPort<ImpulseResponse*>(os.str(),this);
		}
		// Set the proper number of responses
		_responseSpectrums.resize(nChannels);
	}
};

} // namespace CLAM

#endif // AllPassFilterGenerator_hxx

