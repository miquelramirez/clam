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

#ifndef ImpulseResponseLoader_hxx
#define ImpulseResponseLoader_hxx

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
*/
class ImpulseResponseLoader : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, AudioInFilename, ImpulseResponse);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFrameSize(512);
		};
	};

private:
	Config _config;
	OutPort< std::vector<ComplexSpectrum>* > _impulseResponse;
	std::vector<ComplexSpectrum> _responseSpectrums;

public:
	const char* GetClassName() const { return "ImpulseResponseLoader"; }
	ImpulseResponseLoader(const Config& config = Config()) 
		: _impulseResponse("ImpulseResponse", this)
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		std::string errorMsg;
		if (!computeResponseSpectrums( _config.GetImpulseResponse(), _responseSpectrums, _config.GetFrameSize(), errorMsg ))
		{
			AddConfigErrorMessage(errorMsg);
			return false;
		}
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	bool Do()
	{
		_impulseResponse.GetData()= &_responseSpectrums;
		_impulseResponse.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ImpulseResponseLoader_hxx

