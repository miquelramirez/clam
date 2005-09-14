#ifndef _Schema_hxx_
#define _Schema_hxx_

#include "DynamicType.hxx"
#include "HLDSchema.hxx"

namespace CLAM_Annotator{


	class Schema : public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Schema,2);
		DYN_ATTRIBUTE(0, public, HLDSchema, HLDSchema);
		DYN_CONTAINER_ATTRIBUTE(1, public, std::list<std::string>, LLDNames, Names);

		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
		void AddFrameFloatAttribute(const std::string & name)
		{
			GetLLDNames().push_back(name);
		}
	};

};

#endif//_Schema_hxx_
