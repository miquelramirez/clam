#include "MIDIMessage.hxx"

namespace MIDI
{

std::ostream& operator<< (std::ostream &os, const Message& m)
{
	return os << int(m[0]) << ", " << int(m[1]) << ", " << int(m[2]) << ", " << int(m[3]) << std::endl;
}

}