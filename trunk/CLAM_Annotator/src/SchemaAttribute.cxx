#include "SchemaAttribute.hxx"
#include <algorithm>
#include "Pool.hxx"
#include "DescriptionScheme.hxx"
#include "Text.hxx"
#include "IndexArray.hxx"
#include "TypePlugin.hxx"

namespace CLAM_Annotator{

	void SchemaAttribute::DefaultInit()
	{
		AddName();
		AddScope();
		AddType();
		UpdateData();
	}

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

	bool SchemaAttribute::Validate(const CLAM::DescriptionDataPool & pool) const
	{
		Holder<TypePlugin> h(TypePlugin::Create(*this));

		// TODO: Check also when scope size is > 1
		if (h.t) return h.t->ValidateData(pool);

		std::string error = "Validating an unrecognized type: ";
		error += GetType();
		CLAM_ASSERT(false, error.c_str());
		return false;
	}


};
