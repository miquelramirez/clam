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
#include <CLAM/OutControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/DirectoryName.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <algorithm>
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
			std::cout << "Loading " << hrtfFile << "..." << std::endl;
			files.push_back(hrtfFile);
		}
		if (files.empty())
		{
			errorMsg += "Elevation dir: "+path+" contains no suitable .wav files\n";
			return false;
		}
		std::sort(files.begin(), files.end());
		sameElevationIRs.resize( files.size() );
		for (unsigned i=0; i<files.size(); i++)
			if (!computeResponseSpectrums(path+files[i], sameElevationIRs[i], frameSize, errorMsg))
				return false;

		return true;
	}
public:
	std::vector<std::vector<ImpulseResponse> > _storage;
	unsigned NElevation;
	GeodesicDatabase()
		: NElevation(14)
	{
	}
	unsigned NAzimuth(unsigned elevation) 
	{
		return _storage[elevation].size();
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
	ImpulseResponse & get(unsigned elevation, unsigned azimuth)
	{
		return _storage[elevation][azimuth];
	}
};

/**
 @todo document HRTFDatabaseFetcher
 @deprecated Use HRTFDatabaseFetcher2 which uses a database description file instead of using filename patterns.
 @ingroup SpatialAudio
*/
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
	OutPort< ImpulseResponse* > _impulseResponseL;
	OutPort< ImpulseResponse* > _impulseResponseR;
	OutPort< ImpulseResponse* > _previousImpulseResponseL;
	OutPort< ImpulseResponse* > _previousImpulseResponseR;
	FloatInControl _elevation; ///< angle to the horizon
	FloatInControl _azimuth; ///< horizontal angle from viewpoint (north-south-east-west)
	FloatOutControl _chosenElevation; ///< angle to the horizon
	FloatOutControl _chosenAzimuth; ///< horizontal angle from viewpoint (north-south-east-west)
	GeodesicDatabase _database; 
	ImpulseResponse * _previousL;
	ImpulseResponse * _previousR;

public:
	const char* GetClassName() const { return "HRTFDatabaseFetcher"; }
	HRTFDatabaseFetcher(const Config& config = Config()) 
		: _impulseResponseL("ImpulseResponseL", this)
		, _impulseResponseR("ImpulseResponseR", this)
		, _previousImpulseResponseL("PreviousImpulseResponseL", this)
		, _previousImpulseResponseR("PreviousImpulseResponseR", this)
		, _elevation("elevation", this)
		, _azimuth("azimuth", this)
		, _chosenElevation("chosen elevation", this)
		, _chosenAzimuth("chosen azimuth", this)
		, _previousL(0)
		, _previousR(0)
	{
		Configure( config );
		_elevation.SetBounds(-40,90);
		_azimuth.SetBounds(0,360);
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
		std::cout << "HRTF database loaded." << std::endl;
		unsigned elevation = map(_elevation, _database.NElevation, -40, 90);
		unsigned azimuth = map(_azimuth, _database.NAzimuth(elevation), 0, 360);
		_previousL = &_database.get(elevation,azimuth);
		_previousR = &_database.get(elevation,_database.NAzimuth(elevation)-azimuth-1);
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	static unsigned map(InControl & control, unsigned nIndexes, double min, double max)
	{
		double normalizedValue = (control.GetLastValue() - min) / (max-min);
		unsigned result = unsigned(std::floor(normalizedValue*nIndexes+.5));
		return result < nIndexes? result : 0;
	}
	bool Do()
	{
		unsigned elevation = map(_elevation, _database.NElevation, -40, 90);
		unsigned azimuth = map(_azimuth, _database.NAzimuth(elevation), 0, 360);

		_chosenElevation.SendControl(-40+elevation*10);
		_chosenAzimuth.SendControl(azimuth*360./_database.NAzimuth(elevation));

		ImpulseResponse * currentL = &_database.get(elevation,azimuth);
		_impulseResponseL.GetData()= currentL;
		ImpulseResponse * currentR = &_database.get(elevation,_database.NAzimuth(elevation)-azimuth-1);
		_impulseResponseR.GetData()= currentR;
		_previousImpulseResponseL.GetData() = _previousL ? _previousL : currentL;
		_previousImpulseResponseR.GetData() = _previousR ? _previousR : currentR;
		if ( _previousR != currentR) 
			std::cout << "HRTF indices (elevation, azimuth) : "<<elevation<<","<<azimuth<<std::endl;
		_previousL = currentL;
		_previousR = currentR;
		_impulseResponseL.Produce();
		_impulseResponseR.Produce();
		_previousImpulseResponseL.Produce();
		_previousImpulseResponseR.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // HRTFDatabaseFetcher_hxx

