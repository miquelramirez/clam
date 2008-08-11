#ifndef LadspaLibrary_hxx
#define LadspaLibrary_hxx

#include <ladspa.h>
#include <vector>

namespace CLAM
{

/**
 A LadspaLibrary represents a pool of ladspa plugins descriptors.
 You can register new plugin type by calling AddPluginType.
 Normally you want to implicitly add plugins by creating a
 CLAM::LadspaNetworkExporter or a CLAM::LadspaProcessingExporter.
 @see CLAM::LadspaNetworkExporter
 @see CLAM::LadspaProcessingExporter
 @group Ladspa
*/
class LadspaLibrary
{
	std::vector<LADSPA_Descriptor * > _descriptors;
	static void CleanUpDescriptor(LADSPA_Descriptor *& descriptor)
	{
		if (not descriptor) return;
		delete[] descriptor->Label;
		delete[] descriptor->Name;
		delete[] descriptor->Maker;
		delete[] descriptor->Copyright;
		delete[] descriptor->PortDescriptors;

		for (unsigned long lIndex = 0; lIndex < descriptor->PortCount; lIndex++)
			delete[] descriptor->PortNames[lIndex];

		delete[] descriptor->PortNames;
		delete[] descriptor->PortRangeHints;
		delete descriptor;
		descriptor = 0;
	}
public:
	LadspaLibrary()
	{
	}
	~LadspaLibrary()
	{
		for (unsigned i=0; i<_descriptors.size(); i++)
			CleanUpDescriptor(_descriptors[i]);
	}
	void AddPluginType(LADSPA_Descriptor * descriptor)
	{
		_descriptors.push_back(descriptor);
	}
	LADSPA_Descriptor * pluginAt(unsigned long i)
	{
		if (i>=_descriptors.size()) return 0;
		return _descriptors[i];
	}
	static char *dupstr( char const *args )
	{
		const size_t v = strlen(args) + 1;
		char * s = new char[v];
		memcpy( s, args, v);
		return s;
	} 

};
}

#endif//LadspaLibrary_hxx

