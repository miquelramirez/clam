/*
 * Copyright (c) 2010 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef SingleHRTFFetcher_hxx
#define SingleHRTFFetcher_hxx

#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Filename.hxx>
#include "GeodesicHRTFDatabase.hxx"
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <dirent.h>
#include <fstream>
#include <iomanip>

namespace CLAM
{


/**
 A Processing very similar to HRTFDatabaseFetcher with the difference that instead of 
 receiving azimuth and elevation via controls it receives them via configuration.
 That makes possible to just load two HRTFs instead of the whole database.
 Please refere to the HRTFDatabaseFetcher for details.

 @param FrameSize [Config] The framesize to build the ImpulseResponse object
 @param HrtfDatabase [Config] The framesize to build the ImpulseResponse object
 @param[out] "ImpulseResponseL" [Port] The impulse response to be applied on the left channel
 @param[out] "ImpulseResponseR" [Port] The impulse response to be applied on the right channel 
 @param[out] "chosen elevation" [Control] The actual elevation of the chosen HRTF function
 @param[out] "chosen azimuth" [Control] The actual azimuth of the chosen HRTF function
 @todo document SingleHRTFFetcher
 @ingroup SpatialAudio
*/
class SingleHRTFFetcher : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 4, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, InFilename, HrtfDatabase);
		DYN_ATTRIBUTE( 2, public, float, Azimuth);
		DYN_ATTRIBUTE( 3, public, float, Elevation);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetHrtfDatabase( "" );
			SetFrameSize(512);
			SetAzimuth(0.);
			SetElevation(0.);
		};
	};
private:
	
	Config _config;
	OutPort< ImpulseResponse* > _impulseResponseL;
	OutPort< ImpulseResponse* > _impulseResponseR;
	FloatOutControl _chosenElevation; ///< angle to the horizon
	FloatOutControl _chosenAzimuth; ///< horizontal angle from viewpoint (north-south-east-west)
	GeodesicHRTFDatabase _database; 

public:
	const char* GetClassName() const { return "SingleHRTFFetcher"; }
	SingleHRTFFetcher(const Config& config = Config()) 
		: _impulseResponseL("ImpulseResponseL", this)
		, _impulseResponseR("ImpulseResponseR", this)
		, _chosenElevation("chosen elevation", this)
		, _chosenAzimuth("chosen azimuth", this)
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		std::string errorMsg;
		if (!_database.loadImpulseResponseDatabaseMetadata(_config.GetHrtfDatabase(), errorMsg))
		{
			AddConfigErrorMessage(errorMsg);
			return false;
		}
		const unsigned sampleRate=44100;
		if (!_database.loadImpulseResponseOfAnAngle( 
			_config.GetFrameSize(), 
			sampleRate, 
			errorMsg, 
			_config.GetElevation(),
			_config.GetAzimuth() ) )
		{
			AddConfigErrorMessage(errorMsg);
			return false;
		}

		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

public:
	bool Do()
	{
		const double azimuth = _config.GetAzimuth();
		const float elevation = _config.GetElevation();
		unsigned indexL = _database.getIndex(elevation, azimuth);
		unsigned indexR = _database.getIndex(elevation, 360-azimuth);
		_chosenElevation.SendControl(_database.elevationForIndex(indexL));
		_chosenAzimuth.SendControl(_database.azimutForIndex(indexL));

		_impulseResponseL.GetData()= &_database.get(indexL);
		_impulseResponseR.GetData()= &_database.get(indexR);

		_impulseResponseL.Produce();
		_impulseResponseR.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // SingleHRTFFetcher_hxx

