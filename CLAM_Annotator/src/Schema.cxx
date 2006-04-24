#include "Schema.hxx"
#include "Pool.hxx"

namespace CLAM_Annotator
{

	void Schema::InitInstance(const std::string & scope, unsigned instance, CLAM::DescriptionDataPool & data) const
	{
		std::list<SchemaAttribute>& attributes = GetAttributes();
		std::list<SchemaAttribute>::iterator attribute;
		for(attribute = attributes.begin(); attribute != attributes.end(); attribute++)
		{
			if (attribute->GetScope()!=scope) continue;
			attribute->InitInstance(instance, data);
		}
	}
	bool Schema::Validate(const CLAM::DescriptionDataPool& pool, std::ostream & err) const
	{
		const std::list<SchemaAttribute>& attributes = GetAllAttributes();
		std::list<SchemaAttribute>::const_iterator attribute;
		bool validated = true;
		for(attribute = attributes.begin(); attribute != attributes.end(); attribute++)
		{
			validated = validated && attribute->Validate(pool, err);
		}
		return validated;
	}
	void Schema::InitMissingAttributes(CLAM::DescriptionDataPool& pool) const
	{
		const std::list<SchemaAttribute>& attributes = GetAllAttributes();
		std::list<SchemaAttribute>::const_iterator attribute;
		for(attribute = attributes.begin(); attribute != attributes.end(); attribute++)
		{
			if (pool.IsInstantiated(attribute->GetScope(), attribute->GetName())) continue;
			unsigned nInstances = pool.GetNumberOfContexts(attribute->GetScope());
			for (unsigned i=0; i<nInstances; i++)
				attribute->InitInstance(i, pool);
		}
	}
}



