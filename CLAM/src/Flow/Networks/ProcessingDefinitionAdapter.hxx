
#ifndef __PROCESSINGDEFINITIONADAPTER_HXX__
#define __PROCESSINGDEFINITIONADAPTER_HXX__

#include "Component.hxx"
#include <string>

namespace CLAM
{
class Processing;

class ProcessingDefinitionAdapter :public Component
{
private:
	Processing * mAdaptee;
	std::string mName;

public:
	ProcessingDefinitionAdapter( Processing * adaptee = 0, const std::string & name = "");
	virtual ~ProcessingDefinitionAdapter();	
	Processing * GetProcessing(){return mAdaptee;}
	const std::string & GetName(){return mName;}

public:
	//* Returns the class name
	const char * GetClassName() const 
	{
		return "ProcessingDefinitionAdapter";
	}
	
	virtual void StoreOn (Storage & store);

	/**
	 * Loads component's subitems from the given Storage
	 * @param store The given storage where the subitem will be loaded from
	 * @see Storage
	 */
	virtual void LoadFrom (Storage & store);
};

} // namespace CLAM

#endif // __PROCESSINGDEFINITIONADAPTER_HXX__
