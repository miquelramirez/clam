#include "CLT_Descriptor.hxx"

#include <string.h>
#include <stdlib.h>
///////////////////////////
/// Static Declarations ///
///////////////////////////


int CLT_Descriptor::sNumPort=0;
char** CLT_Descriptor::sPortNames = NULL;
LADSPA_PortDescriptor* CLT_Descriptor::sPortDescriptors = NULL;
LADSPA_PortRangeHint * CLT_Descriptor::sPortRangeHints = NULL;

///////////////////////////
///   Implemententation ///
///////////////////////////



CLT_Descriptor::CLT_Descriptor (const unsigned long uniqueID,
								  const char* label,
								  const char* name,
								  const char* maker,
								  const char* copyright,
								  const unsigned long portCount)
{
		UniqueID = uniqueID;
		Label = strdup(label);
		Name = strdup(name);
		Maker = strdup(maker);
		Copyright = strdup(copyright);
		PortCount = portCount;
		Properties = 0;
		sNumPort = 0;

//alloquem memoria pels arrays

		sPortRangeHints = (LADSPA_PortRangeHint *)
				calloc(PortCount, sizeof(LADSPA_PortRangeHint));
		sPortNames = (char **)
				calloc(PortCount, sizeof(char *));
		sPortDescriptors = (LADSPA_PortDescriptor *)
				calloc(PortCount, sizeof(LADSPA_PortDescriptor));

		PortRangeHints = (const LADSPA_PortRangeHint *)sPortRangeHints;
		PortNames = (const char **)sPortNames;
		PortDescriptors = (const LADSPA_PortDescriptor *)sPortDescriptors;
}
		
CLT_Descriptor::~CLT_Descriptor()
{
}

int CLT_Descriptor::ConnectPort(CLT_Port* port)
{

		sPortNames[sNumPort] = port->GetName();
		sPortDescriptors[sNumPort] = port->GetProperties();
		sPortRangeHints[sNumPort] = port->GetPortRangeHint();
//retornem el numero de port que acabem de posar
		sNumPort++;
		return (sNumPort-1);
}

bool CLT_Descriptor::SetProperties (bool RealTime, bool InplaceBroken, bool HardRTCapable)
{
		Properties = 0;
		if (RealTime)
				Properties |= LADSPA_PROPERTY_REALTIME;
		if (InplaceBroken)
				Properties |= LADSPA_PROPERTY_INPLACE_BROKEN;
		if (HardRTCapable)
				Properties |= LADSPA_PROPERTY_HARD_RT_CAPABLE;
		
		return 1;
}

bool CLT_Descriptor::SetProperties (LADSPA_Properties properties)
{
		Properties = properties;
		return 1;
}

CLT_Port::CLT_Port(bool input, bool output, bool audio, bool control, char* name)
{
		mName = name;
		mProperties = 0;
		if (input)
				mProperties |= LADSPA_PORT_INPUT;
		if (output)
				mProperties |= LADSPA_PORT_OUTPUT;
		if (audio)
				mProperties |= LADSPA_PORT_AUDIO;
		if (control)
				mProperties |= LADSPA_PORT_CONTROL;
}

bool CLT_Port::CreateHint(bool boundedBelow, bool boundedAbove, bool toggled, 
					  bool sampleRate, bool logarithmic, bool integer)
{
		mHint.HintDescriptor = 0;
		if (boundedBelow)
				mHint.HintDescriptor |= LADSPA_HINT_BOUNDED_BELOW;
		if (boundedAbove)
				mHint.HintDescriptor |= LADSPA_HINT_BOUNDED_ABOVE;
		if (toggled)
				mHint.HintDescriptor |= LADSPA_HINT_TOGGLED;
		if (sampleRate)
				mHint.HintDescriptor |= LADSPA_HINT_SAMPLE_RATE;
		if (logarithmic)
				mHint.HintDescriptor |= LADSPA_HINT_LOGARITHMIC;
		if (integer)
				mHint.HintDescriptor |= LADSPA_HINT_INTEGER;
		return 1;
}

