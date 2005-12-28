#include "Schema.hxx"

namespace CLAM_Annotator
{

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



