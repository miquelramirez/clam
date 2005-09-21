#include "SchemaAttribute.hxx"
#include <algorithm>
#include "DescriptionScheme.hxx"
#include "Text.hxx"
#include "IndexArray.hxx"

namespace CLAM_Annotator{

	/**************** SchemaAttribute **************/

	void SchemaAttribute::AddTo(CLAM::DescriptionScheme & scheme) const
	{
		const std::string & type = GetType();
		const std::string & name = GetName();
		const std::string & scope = GetScope();
		if (type=="Float")
		{
			scheme.AddAttribute <float>(scope,name);
		}
		else if (type=="Int")
		{
			scheme.AddAttribute <int>(scope,name);
		}
		else if (type=="RestrictedString")
		{
			scheme.AddAttribute <CLAM_Annotator::RestrictedString>(scope,name);
		}
		else if (type=="String")
		{
			scheme.AddAttribute <CLAM::Text>(scope,name);
		}
		else if (type=="Segmentation")
		{
			scheme.AddAttribute<CLAM::IndexArray>(scope,name);
		}
	}

	void SchemaAttribute::DefaultInit()
	{
		AddName();
		AddScope();
		AddType();
		UpdateData();
	}


};
