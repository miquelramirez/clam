#include "SchemaAttribute.hxx"
#include <algorithm>
#include "Pool.hxx"
#include "DescriptionScheme.hxx"
#include "Text.hxx"
#include "IndexArray.hxx"

namespace CLAM_Annotator{

	void SchemaAttribute::DefaultInit()
	{
		AddName();
		AddScope();
		AddType();
		UpdateData();
		mTypePlugin.t = 0;
	}

	void SchemaAttribute::AddTo(CLAM::DescriptionScheme & scheme) const
	{
		UpdateTypePlugin();

		CLAM_ASSERT(mTypePlugin.t,
			(std::string("Adding an unrecognized type: ")+GetType()).c_str());

		mTypePlugin.t->AddTo(scheme);
	}

	bool SchemaAttribute::Validate(const CLAM::DescriptionDataPool & pool) const
	{
		UpdateTypePlugin();
		// TODO: Check also when scope size is > 1
		CLAM_ASSERT(mTypePlugin.t,
			(std::string("Validating an unrecognized type: ")+GetType()).c_str());

		return mTypePlugin.t->ValidateData(pool);
	}


};
