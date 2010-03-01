#ifndef LadspaNetworkExporter_hxx
#define LadspaNetworkExporter_hxx

#include <ladspa.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"
#include "ControlSource.hxx"
#include "ControlSink.hxx"
#include "LadspaLibrary.hxx"


namespace CLAM
{

class LadspaNetworkExporter
{
public:
	LadspaNetworkExporter(LadspaLibrary & library, const std::string & networkXmlContent,
		unsigned id,
		const std::string & label,
		const std::string & name,
		const std::string & maker,
		const std::string & copyright
		);
};

} //namespace CLAM


#endif//LadspaNetworkExporter_hxx

