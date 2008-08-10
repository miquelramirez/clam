#include <CLAM/TypedInControlRegistry.hxx>
#include <CLAM/BaseTypedInControl.hxx>
#include <CLAM/Assert.hxx>

namespace CLAM
{
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
	
	BaseTypedInControl& TypedInControlRegistry::GetByNumber(int index) const
	{
		CLAM_ASSERT(index>=0, "index for Control must be >=0");
		CLAM_ASSERT(index<Size(), "index for Control must be < than Size");

		return *mTypedInControls.at(index);
	}

	bool TypedInControlRegistry::Has(const std::string& name) const
	{
		ConstIterator it;
		for (it=mTypedInControls.begin(); it!=mTypedInControls.end(); it++)
			if(name == (*it)->GetName()) 
				return true;

		return false;
	}

	int TypedInControlRegistry::Size() const
	{
		return mTypedInControls.size();
	}
	TypedInControlRegistry::Iterator  TypedInControlRegistry::Begin()
	{
		return mTypedInControls.begin();
	}
	
	TypedInControlRegistry::Iterator TypedInControlRegistry::End()
	{
		return mTypedInControls.end();
	}
	
	TypedInControlRegistry::ConstIterator  TypedInControlRegistry::Begin() const
	{
		return mTypedInControls.begin();
	}
	
	TypedInControlRegistry::ConstIterator TypedInControlRegistry::End() const
	{
		return mTypedInControls.end();
	}

	void TypedInControlRegistry::ProcessingInterface_Register( BaseTypedInControl * in )
	{
		mTypedInControls.push_back( in );
	}
	
	void TypedInControlRegistry::ProcessingInterface_Unregister( BaseTypedInControl * in )
	{
		for (Iterator it=mTypedInControls.begin(); it!=mTypedInControls.end(); it++)
		{
			if (*it==in)
			{
				// std::cout << "Removing in control "<< in << std::endl;
				mTypedInControls.erase(it);
				return;
			}
		}
	}
	
	
} // namespace CLAM
