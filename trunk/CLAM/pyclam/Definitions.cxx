#include "Definitions.hxx"

namespace CLAM
{

//FIXME: Workaround to get SpecTypeFlags.hxx bindings. From ??? (static)
typedef unsigned int tValue;
tValue SpecTypeFlags::sDefault = 0;

//From Spectrum.cxx (static)
// Flags<4>::tFlagValue SpecTypeFlags::sFlagValues[] = {
//  	{SpecTypeFlags::eComplex, "Complex"},
//  	{SpecTypeFlags::ePolar, "Polar"},
//  	{SpecTypeFlags::eMagPhase, "MagPhase"},
//  	{SpecTypeFlags::eMagPhaseBPF, "MagPhaseBPF"},
//  	{0,NULL}
// };
}

namespace Bindings
{

	bool Network::ConfigureProcessing(const std::string& p, Bindings::ProcessingConfig& c) { 
		return _network.ConfigureProcessing(p, reinterpret_cast<const CLAM::ProcessingConfig&>(c.getReal()));
	}

} // namespace Bindings
