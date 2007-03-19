#ifndef __LadspaWrapperCreator_hxx__
#define __LadspaWrapperCreator_hxx__

#include <CLAM/LadspaWrapper.hxx>

namespace CLAM {

class LadspaWrapperCreator  //TODO move to CLAM/src/Processing/Plugins
{
private:
	std::string _libraryFileName;
	unsigned _index;
public:
	LadspaWrapperCreator( const std::string& libraryFileName, unsigned index ) 
		: _libraryFileName( libraryFileName )
		, _index( index )
	{
	}
	virtual Processing* Create()
	{
		return new CLAM::LadspaWrapper(_libraryFileName, _index);
	}
	virtual ~LadspaWrapperCreator() {};
};

} // namespace CLAM

#endif // __LadspaWrapperCreator_hxx__
