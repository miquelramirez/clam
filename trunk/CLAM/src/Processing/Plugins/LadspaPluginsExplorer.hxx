#ifndef __LADSPAPLUGINSEXPLORER_HXX__
#define __LADSPAPLUGINSEXPLORER_HXX__

#include <string>
#include <list>

namespace CLAM 
{

class LadspaPluginsExplorer {
	public:
		static void ExploreStandardPaths();
		static void ExplorePath(const std::string & path);
};

} // namespace CLAM
#endif //__LADSPAPLUGINSEXPLORER_HXX__

