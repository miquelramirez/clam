
#include "ConnectionDefinitionAdapter.hxx"
#include "Assert.hxx"
#include "XMLAdapter.hxx"

namespace CLAM
{
	ConnectionDefinitionAdapter::ConnectionDefinitionAdapter( const std::string & out, const std::string & in)
		: mOut(out), mIn(in)
	{
	}

	ConnectionDefinitionAdapter::~ConnectionDefinitionAdapter()
	{
	}

	void ConnectionDefinitionAdapter::StoreOn (Storage & store)	
	{
		XMLAdapter<Text> outAdapter( mOut, "out", true);
		XMLAdapter<Text> inAdapter( mIn, "in", true);

		store.Store(&outAdapter);
		store.Store(&inAdapter);
	}

	void ConnectionDefinitionAdapter::LoadFrom (Storage & store) 
	{
		XMLAdapter<Text> outAdapter( mOut, "out", true);
		XMLAdapter<Text> inAdapter( mIn, "in", true);	
		
		store.Load(&outAdapter);
		store.Load(&inAdapter);
	}
} // namespace CLAM
