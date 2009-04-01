#include "SpeakerLayout.hxx"
#include <cmath>
#include <fstream>

void SpeakerLayout::add(double azimuthDegrees, double elevationDegrees, const std::string & name)
{
	std::cout << elevationDegrees << " " << azimuthDegrees << " '" << name << "'" << std::endl;
	_orientations.push_back(CLAM::Orientation(azimuthDegrees, elevationDegrees));
	_names.push_back(name);
}

bool SpeakerLayout::load(
		const std::string & path,
		std::string & errorMsg )
{
	if (path.empty()) return error(errorMsg, "No speaker layout file specified");
	std::ifstream file(path.c_str());
	if (!file) return error(errorMsg, "Could not open the speaker layout file "+path);
	_orientations.clear();
	_names.clear();
	while (true)
	{
		double elevation;
		double azimuth;
		file >> elevation;
		file >> azimuth;
		if (!file) break;
		file >> std::ws;
		std::string name;
		std::getline(file, name);
		// TODO: Issue an error if illegal port name
		add(azimuth, elevation, name);
	}
	return true;
}


