// Version beyond 1.1.0
#include "MIDIMon.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
	static const char * metadata[] = {
		"key", "MIDIMon",
		"category", "MIDI",
		0
		};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MIDIMon> registrator(metadata);	
}
