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
#include <CLAM/Filename.hxx>
#include "Orientation.hxx"
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <map>
#include <dirent.h>
#include <fstream>
#include <iomanip>

namespace CLAM
{

/**
 @internal
 @ingroup SpatialAudio
*/
class GeodesicDatabase
{
	std::vector<ImpulseResponse> _storage;
	std::vector<std::string> _waveFiles;
	std::vector<Orientation> _orientations;
	bool error(std::string & errorMsg, const std::string & message)
	{
		errorMsg += message;
		errorMsg += "\n";
		return false;
	}
public:
	GeodesicDatabase()
	{
	}
	unsigned NAzimuth(unsigned elevation) 
	{
		return _storage[elevation].size();
	}
	std::string baseDir(const std::string & path)
	{
		size_t lastBarPos = path.rfind('/');
		if (lastBarPos == std::string::npos) return "";
		return path.substr(0,lastBarPos+1);
	}
	bool loadImpulseResponseDatabase(
			const std::string & path,
			unsigned frameSize,
			std::string & errorMsg )
	{
		if (path.empty()) return error(errorMsg, "No database file specified");
		std::string base = baseDir(path);

		std::ifstream index(path.c_str());
		if (!index) return error(errorMsg, "Could not open the file "+path);
		_orientations.clear();
		_waveFiles.clear();
		while (true)
		{
			double elevation;
			double azimuth;
			index >> elevation;
			index >> azimuth;
			if (!index) break;
			index >> std::ws;
			std::string filename;
			std::getline(index, filename);
		//	std::cout << elevation << " " << azimuth << " '" << base << filename << "'" << std::endl;
			_orientations.push_back(Orientation(azimuth, elevation));
			_waveFiles.push_back(base+filename);
		}
		_storage.resize(_orientations.size());
		for (unsigned i=0; i < _storage.size(); i++)
			if (!computeResponseSpectrums(_waveFiles[i], _storage[i], frameSize, errorMsg))
				return false;
		return true;
	}
	double azimutForIndex(unsigned index) const
	{
		return _orientations[index].azimuth;
	}
	double elevationForIndex(unsigned index) const
	{
		return _orientations[index].elevation;
	}
	
	ImpulseResponse & get(unsigned index) { return _storage[index]; }
	unsigned getIndex(double elevation, double azimuth)
	{
		Orientation target(azimuth, elevation);
		unsigned chosen = _orientations.size();
		double minDistance = 10000;
		for (unsigned i=0; i < _orientations.size(); i++)
		{
			double distance = _orientations[i].chordDistance(target);
			if (distance>minDistance) continue;
			minDistance = distance;
			chosen = i;
		}
		return chosen;
	}
};

/**
 A Processing that outputs the HRTF of a database that is closer to a given orientation.
 Two ImpulseResponse are fetched, one for the left ear and one for the right one.
 Angles follow @ref AmbisonicsConventions
 
 An HRTF database is a text file, with .hrtfs extension, 
 where each row defines the following space separated fields:
 -# Elevation in degrees
 -# Azimuth in degrees
 -# Path of the wave file containing the left ear response, relative to the path where .hrtf file resides.

 Right ear responses are selected by symmetry.

 @param FrameSize [Config] The framesize to build the ImpulseResponse object
 @param HrtfDatabase [Config] The framesize to build the ImpulseResponse object
 @param[in] elevation [Control] The elevation of the source to be reproduced in listener reference frame
 @param[in] azimuth [Control] The azimuth of the source to be reproduced in listener reference frame
 @param[out] "ImpulseResponseL" [Port] The impulse response to be applied on the left channel
 @param[out] "ImpulseResponseR" [Port] The impulse response to be applied on the right channel 
 @param[out] "chosen elevation" [Control] The actual elevation of the chosen HRTF function
 @param[out] "chosen azimuth" [Control] The actual azimuth of the chosen HRTF function
 @todo document HRTFDatabaseFetcher
 @ingroup SpatialAudio
*/
class HRTFDatabaseFetcher : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, InFilename, HrtfDatabase);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetHrtfDatabase( "" );
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

public:
	const char* GetClassName() const { return "HRTFDatabaseFetcher"; }
	HRTFDatabaseFetcher(const Config& config = Config()) 
		: _impulseResponseL("ImpulseResponseL", this)
		, _impulseResponseR("ImpulseResponseR", this)
		, _elevation("elevation", this)
		, _azimuth("azimuth", this)
		, _chosenElevation("chosen elevation", this)
		, _chosenAzimuth("chosen azimuth", this)
	{
		Configure( config );
		_elevation.SetBounds(-90,90);
		_azimuth.SetBounds(0,360);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		std::string errorMsg;

		if (!_database.loadImpulseResponseDatabase(_config.GetHrtfDatabase(), _config.GetFrameSize(), errorMsg ))
		{
			AddConfigErrorMessage(errorMsg);
			return false;
		}
		std::cout << "HRTF database loaded." << std::endl;
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	static unsigned map(FloatInControl & control, unsigned nIndexes, double min, double max)
	{
		double normalizedValue = (control.GetLastValue() - min) / (max-min);
		unsigned result = unsigned(std::floor(normalizedValue*nIndexes+.5));
		return result < nIndexes? result : 0;
	}
public:
	bool Do()
	{
		double elevation = _elevation.GetLastValue();
		double azimuth = _azimuth.GetLastValue();
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


#endif // HRTFDatabaseFetcher_hxx

