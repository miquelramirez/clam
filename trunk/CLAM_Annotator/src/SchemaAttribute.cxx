#include "SchemaAttribute.hxx"
#include "Text.hxx"
#include <algorithm>

namespace CLAM_Annotator{

	/**************** SchemaAttribute **************/

	void SchemaAttribute::DefaultInit()
	{
		AddName();
		AddScope();
		AddType();
		UpdateData();
	}

	bool SchemaAttribute::Validate(const Descriptor<CLAM_Annotator::RestrictedString>& descriptor)const
	{
		bool validated = true;
		validated = (std::find(GetRestrictionValues().begin(),
			GetRestrictionValues().end(),descriptor.GetValue().GetString())!=
			GetRestrictionValues().end());
		CLAM_DEBUG_ASSERT(validated,
			std::string("Descriptor did not validate: " + descriptor.GetName()).c_str());
		return validated;
		
	}
	
	bool SchemaAttribute::Validate (const Descriptor<int>& descriptor)const
	{
		if (!HasiRange()) return true;
		if (descriptor.GetValue()<GetiRange().GetMin()) return false;
		if (descriptor.GetValue()>GetiRange().GetMax()) return false;
		return true;
	}
	
	bool SchemaAttribute::Validate (const Descriptor<float>& descriptor)const
	{
		if (!HasfRange()) return true;
		if (descriptor.GetValue()<GetfRange().GetMin()) return false;
		if (descriptor.GetValue()>GetfRange().GetMax()) return false;
		return true;
	}

	/**************** Auxiliary functions	**************/

	std::string GetTypeFromValue(int i)
	{
		return "Int";
	}
	std::string GetTypeFromValue(float f)
	{
		return "Float";
	}
	std::string GetTypeFromValue(const CLAM::Text& s)
	{
		return "String";
	}
	std::string GetTypeFromValue(const CLAM_Annotator::RestrictedString& r)
	{
		return "RestrictedString";
	}


};
