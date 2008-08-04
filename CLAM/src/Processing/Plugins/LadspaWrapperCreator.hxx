#ifndef __LadspaWrapperCreator_hxx__
#define __LadspaWrapperCreator_hxx__

#include "LadspaWrapper.hxx"

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
		//std::cout<<"LadspaWrapperCreator() libraryFileName="<<libraryFileName<<std::endl;
	}
	virtual Processing* Create()
	{
		//std::cout << "LadspaWrapperCreator::Create called"<<std::endl;
		return new CLAM::LadspaWrapper(_libraryFileName, _index, _factoryKey);
	}
	virtual ~LadspaWrapperCreator() 
	{
		//std::cout<<"~LadspaWrapperCreator()"<<std::endl; 
	}
};

} // namespace CLAM

#endif // __LadspaWrapperCreator_hxx__

