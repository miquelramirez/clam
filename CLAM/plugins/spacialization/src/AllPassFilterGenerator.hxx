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
#include <CLAM/AudioInFilename.hxx>

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
		DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 1, public, unsigned, NFrames);
		DYN_ATTRIBUTE( 2, public, unsigned, Channels);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetNFrames(4);
			SetFrameSize(512);
		};
	};

private:
	Config _config;
	typedef std::vector<ComplexSpectrum> ImpulseResponse;
	std::vector<ImpulseResponse> _responseSpectrums;
	std::vector< OutPort<ImpulseResponse*> *> _impulseResponseOutputsPorts;

public:
	const char* GetClassName() const { return "AllPassFilterGenerator"; }
	AllPassFilterGenerator(const Config& config = Config()) 
	{
		Configure( config );
	}
	~AllPassFilterGenerator()
	{
		for (unsigned port = 0; port < _impulseResponseOutputsPorts.size(); ++port)
			delete _impulseResponseOutputsPorts[port];
	}
	void fillRandomPhaseSpectrum(ComplexSpectrum & spectrum)
	{
		spectrum.bins[0] = 1.;
		spectrum.bins[spectrum.size()-1]=1.;
		for (unsigned i=1; i<spectrum.nBins-1; i++)
		{
			float phase = 2*M_PI*std::rand()/RAND_MAX;
			spectrum.bins[i] = std::complex<float>(std::cos(phase), std::sin(phase));
		}
		
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		const bool isBformat = (_config.HasIsBFormatFile() and _config.GetIsBFormatFile());
		createOutputs();
		std::string errorMsg;
		const unsigned sampleRate=44100;
		for (unsigned nChannel=0; nChannel<_responseSpectrums.size();nChannel++)
		{
			const bool & OK = computeResponseSpectrums( _config.GetImpulseResponse(), _responseSpectrums[nChannel], _config.GetFrameSize(), errorMsg, nChannel, sampleRate);
			if (not OK)
				return AddConfigErrorMessage(errorMsg);
		}
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	bool Do()
	{
		std::vector< OutPort<ImpulseResponse*> *>::iterator it;
		unsigned i=0;

		for (it=_impulseResponseOutputsPorts.begin(); it!=_impulseResponseOutputsPorts.end();it++)
		{
			(*it)->GetData()= &_responseSpectrums[i];
			(*it)->Produce();
			i++;
		}
		return true;
	}

	void ResizePortsToLayout(unsigned buffersize)
	{
		// Set up the outputs according to the layout
		unsigned speakerToUpdate = 0;
		// delete existing speakers from the first one with different name
		for (unsigned speaker=0 ; speaker<_outputs.size(); speaker++)
			delete _impulseResponseOutputsPorts[speaker];
		_responseSpectrums.clear();
		_impulseResponseOutputsPorts.clear();
		// adding new speakers
		for (unsigned speaker=speakerToUpdate; speaker<_layout.size(); speaker++)
		{
			_impulseResponseOutputsPorts.push_back(new OutPort<ImpulseResponse*>((*it),this)); 
			_responseSpectrums.push_back(ImpulseResponse());
		}
	}
	void createOutputs(const std::vector<std::string> & portNames)
	{
		
		for (unsigned port = 0; port < _impulseResponseOutputsPorts.size(); ++port)
			delete _impulseResponseOutputsPorts[port];
		_impulseResponseOutputsPorts.clear();
		_responseSpectrums.clear();
		std::vector<std::string>::const_iterator it;
		for (it=portNames.begin();it!=portNames.end();it++)
		{
			_impulseResponseOutputsPorts.push_back(new OutPort<ImpulseResponse*>((*it),this)); 
			_responseSpectrums.push_back(ImpulseResponse());
		}
	}
};

} // namespace CLAM

#endif // AllPassFilterGenerator_hxx

