
#include "ProcessingDefinitionAdapter.hxx"
#include "Assert.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Factory.hxx"
#include "XMLAdapter.hxx"

namespace CLAM
{

	typedef Factory<CLAM::Processing> ProcessingFactory;

	ProcessingDefinitionAdapter::ProcessingDefinitionAdapter( Processing * adaptee, const std::string & name )
		:  mAdaptee(adaptee), mName(name)
	{
	}

	ProcessingDefinitionAdapter::~ProcessingDefinitionAdapter()
	{
	}

	void ProcessingDefinitionAdapter::StoreOn (Storage & store)	
	{
		XMLAdapter<std::string> nameAdapter( mName, "id");
		std::string className(mAdaptee->GetClassName());
		XMLAdapter<std::string> classNameAdapter( className, "type");
		store.Store(&nameAdapter);
		store.Store(&classNameAdapter);

		XMLComponentAdapter configAdapter((ProcessingConfig&)mAdaptee->GetConfig());
		store.Store(&configAdapter);
	}

	void ProcessingDefinitionAdapter::LoadFrom (Storage & store) 
	{	
		XMLAdapter<std::string> nameAdapter( mName, "id");
		store.Load(&nameAdapter);
		std::string className("");
		XMLAdapter<std::string> classNameAdapter( className, "type");
		store.Load(&classNameAdapter);

		mAdaptee = ProcessingFactory::GetInstance().Create(className);
		ProcessingConfig & cfg((ProcessingConfig&)mAdaptee->GetConfig());
		XMLComponentAdapter configAdapter( cfg );
		store.Load(&configAdapter);
		mAdaptee->Configure(cfg);
	}
} // namespace CLAM
