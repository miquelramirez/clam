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

#ifndef HRTFDatabaseFetcher_hxx
#define HRTFDatabaseFetcher_hxx

#include <CLAM/InControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/DirectoryName.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>

namespace CLAM
{


class GeodesicDatabase
{
public:
	ImpulseResponse _dummy;
	unsigned NElevation;
	GeodesicDatabase()
		: NElevation(14)
	{
		std::string errMsg;
		if (!computeResponseSpectrums("HRTFs/elev0/L0e000a.wav", _dummy, 512, errMsg))
			std::cerr << errMsg << std::endl;
	}
	unsigned NAzimut(unsigned elevation) 
	{
		return 5*elevation;
	}
	bool loadImpulseResponseDatabase( 
			const std::string & filePrefix,
			unsigned frameSize,
			std::string & errorMsg )
	{
		return true;
	}
	ImpulseResponse & get(unsigned elevation, unsigned azimut)
	{
		return _dummy;
	}
};

class HRTFDatabaseFetcher : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, DirectoryName, Path);
		DYN_ATTRIBUTE( 2, public, std::string, Prefix);
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
	OutPort< ImpulseResponse* > _impulseResponse;
	OutPort< ImpulseResponse* > _previousImpulseResponse;
	InControl _elevation; // angle to the horizon
	InControl _azimut; // horizontal angle from viewpoint (north-south-east-west)
	GeodesicDatabase _database; 
	ImpulseResponse * _previous;

public:
	const char* GetClassName() const { return "HRTFDatabaseFetcher"; }
	HRTFDatabaseFetcher(const Config& config = Config()) 
		: _impulseResponse("ImpulseResponse", this)
		, _previousImpulseResponse("PreviousImpulseResponse", this)
		, _elevation("elevation", this)
		, _azimut("azimut", this)
		, _previous(0)
	{
		Configure( config );
		_elevation.SetBounds(-40,90);
		_azimut.SetBounds(0,360);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		std::string errorMsg;
		if (!_database.loadImpulseResponseDatabase(_config.GetPath() + _config.GetPrefix(), _config.GetFrameSize(), errorMsg ))
		{
			AddConfigErrorMessage(errorMsg);
			return false;
		}
		unsigned elevation = map(_elevation, _database.NElevation, -40, 90);
		unsigned azimut = map(_azimut, _database.NAzimut(elevation), 0, 360);
		_previous = &_database.get(elevation,azimut);
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	static unsigned map(InControl & control, unsigned nIndexes, double min, double max)
	{
		double normalizedValue = (control.GetLastValue() - min) / (max-min);
		unsigned result = unsigned(std::floor(normalizedValue*nIndexes+.5));
		std::cout << normalizedValue << "," << nIndexes << "," << result << std::endl;
		return result < nIndexes? result : 0;
	}
	bool Do()
	{

		unsigned elevation = map(_elevation, _database.NElevation, -40, 90);
		unsigned azimut = map(_azimut, _database.NAzimut(elevation), 0, 360);

		ImpulseResponse * current = &_database.get(elevation,azimut);
		_impulseResponse.GetData()= current;
		_previousImpulseResponse.GetData() = _previous ? _previous : current;
		if ( _previous != current) 
			std::cout << "IR : "<<elevation<<","<<azimut<<std::endl;
		_previous = current;
		_impulseResponse.Produce();
		_previousImpulseResponse.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // HRTFDatabaseFetcher_hxx

