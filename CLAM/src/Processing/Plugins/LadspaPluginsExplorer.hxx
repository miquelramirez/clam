#ifndef __LADSPAPLUGINSEXPLORER_HXX__
#define __LADSPAPLUGINSEXPLORER_HXX__

#include <string>
#include <list>

namespace CLAM 
{

struct LadspaPlugin
{
	std::string libraryFileName;
	unsigned index;
	std::string description;
	std::string label; // store a short name to create the factory ID
	std::string factoryID; 
};

typedef std::list<LadspaPlugin> LadspaPlugins;

class LadspaPluginsExplorer {
	public:
		static LadspaPlugins GetList();
};

} // namespace CLAM
#endif //__LADSPAPLUGINSEXPLORER_HXX__

