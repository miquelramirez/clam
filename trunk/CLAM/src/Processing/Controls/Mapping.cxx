#include <iostream>
#include "Mapping.hxx"

namespace CLAM {

	Mapping* Mapping::Create(const std::string& name)
	{
		for (unsigned int i=0;i<sFactories.size();i++)
		{
			if (sFactories[i]->mName == name)
			{
				return sFactories[i]->Create();
			}
		}
		return 0;
	}

	void MappingFactory::AddMe(void)
	{
		Mapping::sFactories.push_back(this);
	}

	std::vector<MappingFactory*> Mapping::sFactories;

	LinearMappingFactory LinearMappingFactory::sSingleton;
	NoteToFreqMappingFactory NoteToFreqMappingFactory::sSingleton;
	ValueToRatioMappingFactory ValueToRatioMappingFactory::sSingleton;
}

