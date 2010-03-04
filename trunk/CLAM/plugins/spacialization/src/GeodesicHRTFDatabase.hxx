/*
 * Copyright (c) 2007-2010 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef GeodesicHRTFDatabase_hxx
#define GeodesicHRTFDatabase_hxx

#include "Orientation.hxx"
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <dirent.h>
#include <fstream>
#include <iomanip>

namespace CLAM
{

/**
 @internal
 @ingroup SpatialAudio
*/
class GeodesicHRTFDatabase
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
	GeodesicHRTFDatabase()
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
	bool loadImpulseResponseDatabaseMetadata(
			const std::string & path,
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
		return true;
	}
	bool loadImpulseResponseDatabaseData( 
			unsigned frameSize,
			unsigned sampleRate,
			std::string & errorMsg)
	{
		_storage.resize(_orientations.size());
		const unsigned nChannel=0;
		for (unsigned i=0; i < _storage.size(); i++)
			if (!computeResponseSpectrums(_waveFiles[i], _storage[i], frameSize, errorMsg, nChannel, sampleRate))
				return false;
		return true;
	}
	bool loadImpulseResponseOfAnAngle( 
			unsigned frameSize,
			unsigned sampleRate,
			std::string & errorMsg,
			float elevation,
			float azimuth)
	{
		_storage.resize(_orientations.size());
		const unsigned nChannel=0;
		const unsigned indexL=getIndex(elevation, azimuth);
		const unsigned indexR=getIndex(elevation, 360-azimuth);
		if (!computeResponseSpectrums(_waveFiles[indexL], _storage[indexL], frameSize, errorMsg, nChannel, sampleRate))	return false;
		if (!computeResponseSpectrums(_waveFiles[indexR], _storage[indexR], frameSize, errorMsg, nChannel, sampleRate))	return false;
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

} // namespace CLAM

#endif // GeodesicHRTFDatabase_hxx

