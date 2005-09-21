#ifndef _TypePlugin_hxx_
#define _TypePlugin_hxx_

#include <string>

namespace CLAM { class DescriptionDataPool; }
namespace CLAM { class DescriptionScheme; }
namespace CLAM_Annotator { class SchemaAttribute; }

namespace CLAM_Annotator
{
	class TypePlugin
	{
	public:
		TypePlugin(const SchemaAttribute & scheme);
		virtual ~TypePlugin();
//		virtual void AddTo(CLAM::DescriptionScheme & scheme) = 0;
		virtual bool ValidateData(const CLAM::DescriptionDataPool & dataPool) = 0;
		static TypePlugin * Create(const SchemaAttribute & scheme);
	protected:
		const SchemaAttribute & mSchema;
	};

}

#endif//_TypePlugin_hxx_
