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
#include <dirent.h>

namespace CLAM
{


class GeodesicDatabase
{
private:
	bool loadHRTFDir( const std::string & path, unsigned frameSize, std::string & errorMsg)
	{
		_storage.push_back( std::vector<ImpulseResponse>() );
		std::vector<ImpulseResponse> & sameElevationIRs = _storage.back();
		DIR* dir = opendir(path.c_str());
		if (!dir)
		{
			errorMsg += "Can not open HRTF dir: " + path + "\n";
			return false;
		}
		std::vector<std::string> files;
		while ( struct dirent * dirEntry = readdir(dir) )
		{
			std::string hrtfFile(dirEntry->d_name);
			if (hrtfFile[0] != 'L') continue;
			if (hrtfFile.length()>4 && hrtfFile.substr(hrtfFile.length()-4, 4) != ".wav") continue;
			files.push_back(hrtfFile);
		}
		std::cout << path << "  files size "<< files.size() << std::endl;
		if (files.empty())
		{
			errorMsg += "Elevation dir: "+path+" contains no suitable .wav files\n";
			return false;
		}
		std::sort(files.begin(), files.end());
		sameElevationIRs.resize( files.size() );
		for (unsigned i=0; i<files.size(); i++)
			if (!computeResponseSpectrums(path+files[i], sameElevationIRs[i], 512, errorMsg))
				return false;

		return true;
	}
public:
	ImpulseResponse _dummy;
	std::vector<std::vector<ImpulseResponse> > _storage;
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
		return _storage[0].size();
	}
	bool loadImpulseResponseDatabase( 
			const std::string & path,
			unsigned frameSize,
			std::string & errorMsg )
	{
		for (int i=-40; i<=90; i+=10) //TODO use config
		{
			std::ostringstream os;
			os << path << "elev" << i << "/" << std::flush;
			if (!loadHRTFDir( os.str(), frameSize, errorMsg ))
				return false;
		}
		return true;
	}
	ImpulseResponse & get(unsigned elevation, unsigned azimut)
	{
		return _storage[0][azimut];
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

		_config.SetPrefix( "" ); //TODO remove
		_config.SetPath( "HRTFs/" ); //TODO remove
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

