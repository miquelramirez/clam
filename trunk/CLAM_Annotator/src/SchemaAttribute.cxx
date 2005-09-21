#include "SchemaAttribute.hxx"
#include <algorithm>
#include "DescriptionScheme.hxx"
#include "Text.hxx"
#include "IndexArray.hxx"

namespace CLAM_Annotator{

	/**************** SchemaAttribute **************/

	void SchemaAttribute::AddTo(CLAM::DescriptionScheme & scheme) const
	{
		Holder<TypePlugin> h(TypePlugin::Create(*this));
		if (h.t)
		{
			h.t->AddTo(scheme);
			return;
		}

		std::string error = "Adding an unrecognized type: ";
		error += GetType();
		CLAM_ASSERT(false, error.c_str());
	}

	void SchemaAttribute::DefaultInit()
	{
		AddName();
		AddScope();
		AddType();
		UpdateData();
	}


};
