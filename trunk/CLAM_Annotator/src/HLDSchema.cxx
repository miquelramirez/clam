#include "HLDSchema.hxx"
#include "Text.hxx"
#include <algorithm>

namespace CLAM_Annotator{

	/**************** HLDSchemaElement **************/

	void HLDSchemaElement::DefaultInit()
	{
		AddName();
		AddScope();
		AddType();
		UpdateData();
	}

	bool HLDSchemaElement::Validate(const Descriptor<CLAM_Annotator::RestrictedString>& descriptor)const
	{
		bool validated = true;
		validated = (std::find(GetRestrictionValues().begin(),
			GetRestrictionValues().end(),descriptor.GetValue().GetString())!=
			GetRestrictionValues().end());
		CLAM_DEBUG_ASSERT(validated,
			std::string("Descriptor did not validate: " + descriptor.GetName()).c_str());
		return validated;
		
	}
	
	bool HLDSchemaElement::Validate (const Descriptor<int>& descriptor)const
	{
		bool validated = true;
		validated = (descriptor.GetValue()>=GetiRange().GetMin()&&
			descriptor.GetValue()<=GetiRange().GetMax());
		CLAM_DEBUG_ASSERT(validated,
			std::string("Descriptor did not validate: " + descriptor.GetName()).c_str());
		return validated;
	}
	
	bool HLDSchemaElement::Validate (const Descriptor<float>& descriptor)const
	{
		bool validated = true;
		validated = (descriptor.GetValue()>=GetfRange().GetMin()&&
			descriptor.GetValue()<=GetfRange().GetMax());
		CLAM_DEBUG_ASSERT(validated,
			std::string("Descriptor did not validate: " + descriptor.GetName()).c_str());
		return validated;
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
