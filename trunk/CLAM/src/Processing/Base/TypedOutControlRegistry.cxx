#include <CLAM/TypedOutControlRegistry.hxx>
#include <CLAM/BaseTypedOutControl.hxx>
#include <CLAM/Assert.hxx>

namespace CLAM
{
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
	
	BaseTypedOutControl& TypedOutControlRegistry::GetByNumber(int index) const
	{
		CLAM_ASSERT(index>=0, "index for Control must be >=0");
		CLAM_ASSERT(index<Size(), "index for Control must be < than Size");

		return *mTypedOutControls.at(index);
	}
	
	bool TypedOutControlRegistry::Has(const std::string& name) const
	{
		ConstIterator it;
		for (it=mTypedOutControls.begin(); it!=mTypedOutControls.end(); it++)
			if(name == (*it)->GetName()) 
				return true;

		return false;
	}

	int TypedOutControlRegistry::Size() const
	{
		return mTypedOutControls.size();
	}

	void TypedOutControlRegistry::ProcessingInterface_Register( BaseTypedOutControl * in )
	{
		mTypedOutControls.push_back( in );
	}
	
	void TypedOutControlRegistry::ProcessingInterface_Unregister( BaseTypedOutControl * out )
	{
		for (Iterator it=mTypedOutControls.begin(); it!=mTypedOutControls.end(); it++)
		{
			if (*it==out)
			{
				// std::cout << "Removing out control "<< out << std::endl;
				mTypedOutControls.erase(it);
				return;
			}
		}
	}	
} // namespace CLAM
