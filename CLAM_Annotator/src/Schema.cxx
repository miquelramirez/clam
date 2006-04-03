#include "Schema.hxx"

namespace CLAM_Annotator
{

	void Schema::InitInstance(const std::string & scope, unsigned instance, CLAM::DescriptionDataPool & data)
	{
		std::list<SchemaAttribute>& attributes = GetAttributes();
		std::list<SchemaAttribute>::iterator attribute;
		for(attribute = attributes.begin(); attribute != attributes.end(); attribute++)
		{
			if (attribute->GetScope()!=scope) continue;
			attribute->InitInstance(instance, data);
		}
	}
	bool Schema::Validate(const CLAM::DescriptionDataPool& pool, std::ostream & err)
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
}



