#ifndef __LadspaWrapperCreator_hxx__
#define __LadspaWrapperCreator_hxx__

#include <CLAM/LadspaWrapper.hxx>

namespace CLAM {

class LadspaWrapperCreator : public CLAM::Factory<Processing>::Creator 
{
private:
	std::string _libraryFileName;
	unsigned _index;
	std::string _factoryKey;
public:
	LadspaWrapperCreator( const std::string& libraryFileName, unsigned index, const std::string& key ) 
		: _libraryFileName( libraryFileName )
		, _index( index )
		, _factoryKey( key )
	{
	}
	virtual Processing* Create()
	{
		return new CLAM::LadspaWrapper(_libraryFileName, _index, _factoryKey);
	}
	virtual ~LadspaWrapperCreator() {};
};

} // namespace CLAM

#endif // __LadspaWrapperCreator_hxx__

