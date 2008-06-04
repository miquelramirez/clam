#include <CLAM/TypedInControlRegistry.hxx>
#include <CLAM/TypedInControl.hxx>
#include <CLAM/Assert.hxx>

namespace CLAM {
	std::string TypedInControlRegistry::AvailableNames() const
	{
		std::string result;
		std::string separator = "";
		for (ConstIterator it=mTypedInControls.begin(); it!=mTypedInControls.end(); it++)
		{
			BaseTypedInControl & control = *(*it);
			result += separator + "'" + control.GetName() + "'";
			separator = ",";
		}
		return result;
	}
	
	BaseTypedInControl& TypedInControlRegistry::Get(const std::string & name) const
	{
		ConstIterator it;
		for (it=mTypedInControls.begin(); it!=mTypedInControls.end(); it++)
			if (name == (*it)->GetName())
				return **it;

		std::string error = 
			"No typed in control named '" +  name + "'.\nTry with: " + AvailableNames();
		CLAM_ASSERT( false, error.c_str() );

		return *(BaseTypedInControl*)NULL; // just to get rid of warnings
	}


	void TypedInControlRegistry::ProcessingInterface_Register( BaseTypedInControl * in )
	{
		mTypedInControls.push_back( in );
	}
	
} // namespace CLAM
