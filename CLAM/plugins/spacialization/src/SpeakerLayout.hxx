#ifndef SpeakerLayout_hxx
#define SpeakerLayout_hxx
#include "Orientation.hxx"
#include <cmath>
#include <fstream>

/**
 An spatial layout of labeled speakers.
 A layout contains the orientation of each speaker referred to the listener or the center of the setup.
 A layout can be read from a .layout file which contains a row for each speaker and for each row:
 # elevation in degrees
 # azimuth in degrees
 # the speaker label (may containing spaces as it takes the rest of the line)
 Empty lines are ignored. 
*/
class SpeakerLayout
{
	std::vector<std::string> _names;
	std::vector<CLAM::Orientation> _orientations;
	bool error(std::string & errorMsg, const std::string & message)
	{
		errorMsg += message;
		errorMsg += "\n";
		return false;
	}
public:
	unsigned size() const { return _names.size(); }
	const std::string & name(unsigned i) const { return _names[i]; }
	const CLAM::Orientation & orientation(unsigned i) const { return _orientations[i]; }
	void add(double azimuthDegrees, double elevationDegrees, const std::string & name);
	bool load(const std::string & path, std::string & errorMsg);
	void clear()
	{
		_names.clear();
		_orientations.clear();
	}
};
#endif

