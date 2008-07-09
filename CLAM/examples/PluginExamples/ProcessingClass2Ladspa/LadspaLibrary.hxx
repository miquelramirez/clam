#ifndef LadspaLibrary_hxx
#define LadspaLibrary_hxx

#include <ladspa.h>
#include <vector>

namespace CLAM
{
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
#if 0
		free((char *)descriptor->Label);
		free((char *)descriptor->Name);
		free((LADSPA_PortDescriptor *)descriptor->PortDescriptors);
		for (unsigned long lIndex = 0; lIndex < descriptor->PortCount; lIndex++)
			 free((char *)(descriptor->PortNames[lIndex]));
		free((char **)descriptor->PortNames);
		free((LADSPA_PortRangeHint *)descriptor->PortRangeHints);
		free(descriptor);
		descriptor=0;
#endif
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

