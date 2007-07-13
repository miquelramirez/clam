#ifndef __LADSPAPLUGINSEXPLORER_HXX__
#define __LADSPAPLUGINSEXPLORER_HXX__

#include <string>
#include <list>

namespace CLAM 
{

//TODO remove.
struct LadspaPlugin
{
	std::string libraryFileName;
	unsigned index;
	std::string description;
	std::string label; // store a short name to create the factory ID
	std::string factoryID; 
};


class LadspaPluginsExplorer {
	public:
		void ExploreStandardPaths();
		void ExplorePath(const std::string & path);
};

} // namespace CLAM
#endif //__LADSPAPLUGINSEXPLORER_HXX__

