#include "SpeakerLayout.hxx"
#include <cmath>
#include <fstream>

void SpeakerLayout::add(double azimuthDegrees, double elevationDegrees, const std::string & name)
{
//	std::cout << elevationDegrees << " " << azimuthDegrees << " '" << name << "'" << std::endl;
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
	clear();
	for (unsigned i=0; true; i++)
	{
		std::string line;
		getline(file,line);
		if (file.eof()) break;
		if (line[0]=='#') continue;
		std::ostringstream os;
		os << i;
		std::istringstream is(line);
		double elevation;
		double azimuth;
		if (not (is >> elevation))
			return error(errorMsg, "Expected elevation at line "+os.str());
		if (not (is >> azimuth))
			return error(errorMsg, "Expected azimuth at line "+os.str());
		is >> std::ws;
		std::string name;
		std::getline(is, name);
		// TODO: Issue an error if illegal port name
		add(azimuth, elevation, name);
	}
	return true;
}


