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

	bool SchemaAttribute::Validate(const CLAM::DescriptionDataPool & pool, std::ostream & err) const
	{
		UpdateTypePlugin();
		// TODO: Check also when scope size is > 1
		if (!mTypePlugin.t)
		{
			err 
				<< "Attribute '" << GetScope() << ":" << GetName() 
				<< "' has type '" << GetType() << "' "
				<< "which is not supported by this program."
				<< std::endl;
			return false;
		}
		std::ostringstream subErr;
		if (mTypePlugin.t->ValidateData(pool,subErr)) return true;
		err << "Error validating attribute '" << GetScope() << ":" << GetName() << "':" << std::endl;
		err << subErr.str();
		return false;
	}


};
