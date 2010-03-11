#ifndef LV2Library_hxx
#define LV2Library_hxx

#include <vector>
#include <lv2.h>
#include <map>
#include <cstring>

/**
 Provides a set of LV2 (LADSPA v2) plugins to be hosted by a library.
 @todo Document final code for the user to write to define a LV2 plugin.
 @ingroup Plugins
 @ingroup LV2
*/
class LV2Library
{
	std::vector<LV2_Descriptor * > _descriptors;

public:
	LV2Library()
	{
	}
	~LV2Library()
	{
		for (unsigned i=0; i<_descriptors.size(); i++)
			CleanUpDescriptor(_descriptors[i]);
	}
	
	void AddPluginType(LV2_Descriptor * descriptor, const std::string & networkContent)
	{
		_descriptors.push_back(descriptor);
		addUri(descriptor->URI,networkContent);
	}
	LV2_Descriptor * pluginAt(unsigned long i)
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
	static std::string & getNetwork(const std::string & uri)	
	{
		return uri2network()[uri];
	}
private:
	static void addUri(const std::string & uri,const std::string & network)
	{
		uri2network()[uri] = network;	
	}
	static void CleanUpDescriptor(LV2_Descriptor *& descriptor)
	{
		if (not descriptor) return;
		delete[] descriptor->URI;
		delete descriptor;
		descriptor = 0;
	}
	static std::map<std::string,std::string> & uri2network()
	{
		static std::map<std::string,std::string> map;
		return map;	
	} 
};

#endif//LV2Library_hxx

