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

#ifndef HRTFDatabaseFetcher2_hxx
#define HRTFDatabaseFetcher2_hxx

#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Filename.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <dirent.h>
#include <fstream>

namespace CLAM
{


class GeodesicDatabase2
{
	static double angularDistance(float azimut1, float elevation1, float azimut2, float elevation2)
	{
		// From http://www.codeguru.com/Cpp/Cpp/algorithms/article.php/c5115/
		// cos (90 - lat2) * cos (90 - lat1) + sin (90 - lat2) * sin (90 - lat1) * cos (lon2 - lon1)
		// taking that elevation = 90 - latitude
		return std::acos(std::cos(elevation1)*std::cos(elevation2) + std::sin(elevation1)*std::sin(elevation2)*std::cos(azimut2-azimut1));
		// TODO: Possible optimization: precompute cos and sinus
		// cos a1-a2 = cos a1 cos a2 + sin a1 sin a2
		// so:   acos( ce1*ce2 + se1*se2 *(sa1 sa2 + ca1 ca2) )
	}
	std::vector<std::vector<ImpulseResponse> > _storage;
	bool error(std::string & errorMsg, const std::string & message)
	{
		errorMsg += message;
		errorMsg += "\n";
		return false;
	}
public:
	unsigned NElevation;
	GeodesicDatabase2()
		: NElevation(14)
	{
	}
	unsigned NAzimut(unsigned elevation) 
	{
		return _storage[elevation].size();
	}
	bool loadImpulseResponseDatabase(
			const std::string & path,
			unsigned frameSize,
			std::string & errorMsg )
	{
		if (path.empty()) return error(errorMsg, "No database file specified");

		std::ifstream index(path.c_str());
		if (!index) return error(errorMsg, "Could not open the file "+path);

		while (true)
		{
			double elevation;
			double azimut;
			index >> elevation;
			index >> azimut;
			if (!index) return true;
			std::string filename;
			std::getline(index, filename);
			std::cout << elevation << " " << azimut << " " <<  filename << std::endl;
			ImpulseResponse ir;
			if (false and !computeResponseSpectrums(filename, ir, frameSize, errorMsg))
				return false;
		}
		return false;
	}
	ImpulseResponse & get(unsigned elevation, unsigned azimut)
	{
		return _storage[elevation][azimut];
	}
};

class HRTFDatabaseFetcher2 : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, InFilename, Path);
		DYN_ATTRIBUTE( 2, public, std::string, Prefix);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetPrefix( "" );
			SetPath( "" );
			SetFrameSize(512);
		};
	};
private:
	
	Config _config;
	OutPort< ImpulseResponse* > _impulseResponseL;
	OutPort< ImpulseResponse* > _impulseResponseR;
	OutPort< ImpulseResponse* > _previousImpulseResponseL;
	OutPort< ImpulseResponse* > _previousImpulseResponseR;
	InControl _elevation; ///< angle to the horizon
	InControl _azimut; ///< horizontal angle from viewpoint (north-south-east-west)
	OutControl _chosenElevation; ///< angle to the horizon
	OutControl _chosenAzimut; ///< horizontal angle from viewpoint (north-south-east-west)
	GeodesicDatabase2 _database; 
	ImpulseResponse * _previousL;
	ImpulseResponse * _previousR;

public:
	const char* GetClassName() const { return "HRTFDatabaseFetcher2"; }
	HRTFDatabaseFetcher2(const Config& config = Config()) 
		: _impulseResponseL("ImpulseResponseL", this)
		, _impulseResponseR("ImpulseResponseR", this)
		, _previousImpulseResponseL("PreviousImpulseResponseL", this)
		, _previousImpulseResponseR("PreviousImpulseResponseR", this)
		, _elevation("elevation", this)
		, _azimut("azimut", this)
		, _chosenElevation("chosen elevation", this)
		, _chosenAzimut("chosen azimut", this)
		, _previousL(0)
		, _previousR(0)
	{
		Configure( config );
		_elevation.SetBounds(-40,90);
		_azimut.SetBounds(0,360);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		std::string errorMsg;

		if (!_database.loadImpulseResponseDatabase(_config.GetPath(), _config.GetFrameSize(), errorMsg ))
		{
			AddConfigErrorMessage(errorMsg);
			return false;
		}
		std::cout << "HRTF database loaded." << std::endl;
		unsigned elevation = map(_elevation, _database.NElevation, -40, 90);
		unsigned azimut = map(_azimut, _database.NAzimut(elevation), 0, 360);
		_previousL = &_database.get(elevation,azimut);
		_previousR = &_database.get(elevation,_database.NAzimut(elevation)-azimut-1);
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
		unsigned azimut = map(_azimut, _database.NAzimut(elevation), 0, 360);

		_chosenElevation.SendControl(-40+elevation*10);
		_chosenAzimut.SendControl(azimut*360./_database.NAzimut(elevation));

		ImpulseResponse * currentL = &_database.get(elevation,azimut);
		_impulseResponseL.GetData()= currentL;
		ImpulseResponse * currentR = &_database.get(elevation,_database.NAzimut(elevation)-azimut-1);
		_impulseResponseR.GetData()= currentR;
		_previousImpulseResponseL.GetData() = _previousL ? _previousL : currentL;
		_previousImpulseResponseR.GetData() = _previousR ? _previousR : currentR;
		if ( _previousR != currentR) 
			std::cout << "HRTF indices (elevation, azimut) : "<<elevation<<","<<azimut<<std::endl;
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


#endif // HRTFDatabaseFetcher2_hxx

