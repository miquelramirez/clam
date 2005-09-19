#include "Schema.hxx"

namespace CLAM_Annotator
{

	bool Schema::Validate(const CLAM::DescriptionDataPool& pool)
	{
		const std::list<CLAM_Annotator::SchemaAttribute>& hlds = GetAllAttributes();
		std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it2;
		bool validated = true;
		for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
		{
			// TODO: Chekc also when scope size is > 1
			const std::string & type = it2->GetType();
			const std::string & name = it2->GetName();
			const std::string & scope = it2->GetScope();
			if(type=="Int")
				validated = validated && 
					ValidateDescriptor(scope, name,
						pool.GetReadPool<int>(scope,name));
			else if(type=="Float")
				validated = validated && 
					ValidateDescriptor(scope, name,
						pool.GetReadPool<float>(scope,name));
			else if(type=="RestrictedString")
				validated = validated && 
					ValidateDescriptor(scope, name,
						pool.GetReadPool<RestrictedString>(scope,name));
			else if(type=="String")
				validated = validated &&
					ValidateDescriptor(scope, name,
						pool.GetReadPool<CLAM::Text>(scope,name));
			CLAM_ASSERT(validated,"NotValidated");
		}
		return validated;
	}

	SchemaAttribute Schema::FindElement(const std::string& scope, const std::string& descriptorName) const
	{
		const std::list<SchemaAttribute> & hlds = GetAllAttributes();
		std::list<SchemaAttribute>::const_iterator it;
		for(it = hlds.begin(); it != hlds.end(); it++)
		{
			if (it->GetName() == descriptorName)
				return (*it);
		}
		CLAM_ASSERT(false, "Descriptor not found in Scheme");
	}


}



