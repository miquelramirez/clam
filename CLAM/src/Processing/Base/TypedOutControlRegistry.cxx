#include <CLAM/TypedOutControlRegistry.hxx>
#include <CLAM/TypedOutControl.hxx>
#include <CLAM/Assert.hxx>

namespace CLAM {
	std::string TypedOutControlRegistry::AvailableNames() const
	{
		std::string result;
		std::string separator = "";
		for (ConstIterator it=mTypedOutControls.begin(); it!=mTypedOutControls.end(); it++)
		{
			BaseTypedOutControl & control = *(*it);
			result += separator + "'" + control.GetName() + "'";
			separator = ",";
		}
		return result;
	}
	
	BaseTypedOutControl& TypedOutControlRegistry::Get(const std::string & name) const
	{
		ConstIterator it;
		for (it=mTypedOutControls.begin(); it!=mTypedOutControls.end(); it++)
			if (name == (*it)->GetName())
				return **it;

		std::string error = 
			"No typed in control named '" +  name + "'.\nTry with: " + AvailableNames();
		CLAM_ASSERT( false, error.c_str() );

		return *(BaseTypedOutControl*)NULL; // just to get rid of warnings
	}

	void TypedOutControlRegistry::ProcessingInterface_Register( BaseTypedOutControl * in )
	{
		mTypedOutControls.push_back( in );
	}
	
} // namespace CLAM
