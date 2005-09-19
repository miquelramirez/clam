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

	bool SchemaAttribute::Validate(const CLAM_Annotator::RestrictedString * descriptor) const
	{
		return std::find(GetRestrictionValues().begin(),
			GetRestrictionValues().end(),descriptor[0].GetString())!=
			GetRestrictionValues().end();
	}
	
	bool SchemaAttribute::Validate (const int * descriptor) const
	{
		if (!HasiRange()) return true;
		if (descriptor[0]<GetiRange().GetMin()) return false;
		if (descriptor[0]>GetiRange().GetMax()) return false;
		return true;
	}
	
	bool SchemaAttribute::Validate (const float * descriptor) const
	{
		if (!HasfRange()) return true;
		if (descriptor[0]<GetfRange().GetMin()) return false;
		if (descriptor[0]>GetfRange().GetMax()) return false;
		return true;
	}


};
