#include "Schema.hxx"

namespace CLAM_Annotator
{

	bool Schema::Validate(const CLAM::DescriptionDataPool& pool)
	{
		std::list<CLAM_Annotator::HLDSchemaElement>& hlds = GetHLDSchema().GetHLDs();
		std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
		bool validated = true;
		for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
		{
			// TODO: Chekc also when scope size is > 1
			const std::string & type = it2->GetType();
			const std::string & name = it2->GetName();
			const std::string & scope = it2->GetScope();
			if(type=="Int")
				validated = validated && 
					GetHLDSchema().ValidateDescriptor(MakeDescriptor(
						*pool.GetReadPool<int>(scope,name),name));
			else if(type=="Float")
				validated = validated && 
					GetHLDSchema().ValidateDescriptor(MakeDescriptor(
						*pool.GetReadPool<float>(scope,name),name));
			else if(type=="RestrictedString")
				validated = validated && 
					GetHLDSchema().ValidateDescriptor(MakeDescriptor(
						*pool.GetReadPool<RestrictedString>(scope,name),name));
			else if(type=="String")
				validated = validated &&
					GetHLDSchema().ValidateDescriptor(MakeDescriptor(
						*pool.GetReadPool<CLAM::Text>(scope,name),name));
			CLAM_ASSERT(validated,"NotValidated");
		}
		return validated;
	}

}



