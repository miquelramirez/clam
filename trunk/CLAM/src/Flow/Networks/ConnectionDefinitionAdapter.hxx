

#ifndef __CONNECTIONDEFINITIONADAPTER_HXX__
#define __CONNECTIONDEFINITIONADAPTER_HXX__

#include "Component.hxx"
#include "Text.hxx"
#include <string>

namespace CLAM
{

class ConnectionDefinitionAdapter :public Component
{
private:
	Text mOut;
	Text mIn;

public:
	ConnectionDefinitionAdapter( const std::string & out = "", const std::string & in = "");
	virtual ~ConnectionDefinitionAdapter();	
	const std::string & GetOutName(){return mOut;}
	const std::string & GetInName(){return mIn;}

public:
	//* Returns the class name
	const char * GetClassName() const 
	{
		return "ConnectionDefinitionAdapter";
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

#endif // __CONNECTIONDEFINITIONADAPTER_HXX__
