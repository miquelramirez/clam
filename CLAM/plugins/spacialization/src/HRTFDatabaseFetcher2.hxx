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
#include <map>
#include <dirent.h>
#include <fstream>
#include <iomanip>

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
	class SpherePosition
	{
	public:
		SpherePosition(int elevation, double azimuth, const std::string & filename)
			: elevation(elevation)
			, azimuth(azimuth)
			, eradians(M_PI*elevation/180.)
			, aradians(M_PI*azimuth/180.)
			, ce(std::cos(eradians))
			, se(std::sin(eradians))
			, ca(std::cos(aradians))
			, sa(std::sin(aradians))
			, filename(filename)
		{
		}
		int elevation;
		double azimuth;
		double eradians, aradians;
		double ce, se, ca, sa;
		std::string filename;
		double angularDistanceTo(const SpherePosition & line)
		{
			double dz = se-line.se;
			double dy = ce*sa-line.ce*line.sa;
			double dx = ce*ca-line.ce*line.ca;
			return dx*dx + dy*dy + dz*dz;
			return std::acos(ce*line.ce+se*line.se*(ca*line.ca+sa*line.sa));
		}
	};
	std::vector<ImpulseResponse> _storage;
	std::vector<SpherePosition> _lines;
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
			_lines.push_back(SpherePosition(elevation, azimuth, base+filename));
		}
		_storage.resize(_lines.size());
		for (unsigned i=0; i < _storage.size(); i++)
			if (!computeResponseSpectrums(_lines[i].filename, _storage[i], frameSize, errorMsg))
				return false;
		return true;
	}
	double azimutForIndex(unsigned index) const
	{
		return _lines[index].azimuth;
	}
	double elevationForIndex(unsigned index) const
	{
		return _lines[index].elevation;
	}
	
	ImpulseResponse & get(unsigned index) { return _storage[index]; }
	unsigned getIndex(double elevation, double azimuth)
	{
		SpherePosition target(elevation, azimuth, "dummy");
		unsigned chosen = _lines.size();
		double minDistance = 10000;
		for (unsigned i=0; i < _lines.size(); i++)
		{
			double distance = _lines[i].angularDistanceTo(target);
			if (distance>minDistance) continue;
			minDistance = distance;
			chosen = i;
		}
		return chosen;
	}
};

/**
 @todo document HRTFDatabaseFetcher2
 @ingroup SpatialAudio
*/
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
	FloatInControl _elevation; ///< angle to the horizon
	FloatInControl _azimuth; ///< horizontal angle from viewpoint (north-south-east-west)
	FloatOutControl _chosenElevation; ///< angle to the horizon
	FloatOutControl _chosenAzimuth; ///< horizontal angle from viewpoint (north-south-east-west)
	GeodesicDatabase2 _database; 
	ImpulseResponse * _previousL;
	ImpulseResponse * _previousR;

public:
	const char* GetClassName() const { return "HRTFDatabaseFetcher2"; }
	HRTFDatabaseFetcher2(const Config& config = Config()) 
		: _impulseResponseL("ImpulseResponseL", this)
		, _impulseResponseR("ImpulseResponseR", this)
		, _elevation("elevation", this)
		, _azimuth("azimuth", this)
		, _chosenElevation("chosen elevation", this)
		, _chosenAzimuth("chosen azimuth", this)
		, _previousL(0)
		, _previousR(0)
	{
		Configure( config );
		_elevation.SetBounds(-90,90);
		_azimuth.SetBounds(0,360);
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
		_previousL = 0;
		_previousR = 0;
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

		ImpulseResponse * currentL = _impulseResponseL.GetData()= &_database.get(indexL);
		ImpulseResponse * currentR = _impulseResponseR.GetData()= &_database.get(indexR);

		if ( _previousL != currentL) 
		{
		//	std::cout << "HRTF (elevation, azimuth) : "<<elevation<<","<<azimuth<<std::endl;
		//	std::cout << "L : "<<_database.elevationForIndex(indexL)<<","<<_database.azimutForIndex(indexL)<<std::endl;
		//	std::cout << "R : "<<_database.elevationForIndex(indexR)<<","<<_database.azimutForIndex(indexR)<<std::endl;
		}
		_previousL = currentL;
		_previousR = currentR;
		_impulseResponseL.Produce();
		_impulseResponseR.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // HRTFDatabaseFetcher2_hxx

