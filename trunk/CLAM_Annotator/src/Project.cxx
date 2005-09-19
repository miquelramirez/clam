#include "Project.hxx"
#include "Text.hxx"
#include "RestrictedString.hxx"
#include "IndexArray.hxx"
#include "XMLStorage.hxx"


namespace CLAM_Annotator
{

void Project::CreatePoolScheme()
{
	mDescriptionScheme = CLAM::DescriptionScheme();
	//First we start with HLD
	const std::list<CLAM_Annotator::SchemaAttribute>& hlds = mSchema.GetAllAttributes();
	std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it2;
	for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
	{
		const std::string & type = it2->GetType();
		const std::string & name = it2->GetName();
		const std::string & scope = it2->GetScope();
		if (type=="Float")
		{
			mDescriptionScheme.AddAttribute <float>(scope,name);
		}
		else if (type=="Int")
		{
			mDescriptionScheme.AddAttribute <int>(scope,name);
		}
		else if (type=="RestrictedString")
		{
			mDescriptionScheme.AddAttribute <CLAM_Annotator::RestrictedString>(scope,name);
		}
		else if (type=="String")
		{
			mDescriptionScheme.AddAttribute <CLAM::Text>(scope,name);
		}
		else if (type=="Segmentation")
		{
			mDescriptionScheme.AddAttribute<CLAM::IndexArray>(scope,name);
		}
	}
}

bool Project::LoadScheme(const std::string & schemeFileName)
{
	CLAM_Annotator::Schema tempSchema;
	CLAM::XMLStorage::Restore(tempSchema,schemeFileName); // May throw an exception

	// Successfull file, just change it
	SetSchema(std::string(schemeFileName)); // temp created to support self assigns
	GetAnnotatorSchema() = tempSchema;
	GetDescriptionScheme() = CLAM::DescriptionScheme();
	CreatePoolScheme();
	return true;
}

Project::ScopeSchema Project::GetScopeSchema(const std::string & scope) const
{
	Project::ScopeSchema scopeSchema;
	const std::list<CLAM_Annotator::SchemaAttribute>& hlds = mSchema.GetAllAttributes();
	std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it;
	for(it = hlds.begin(); it != hlds.end(); it++)
	{
		if (it->GetScope()!=scope) continue;
		if (it->GetType()=="Segmentation") continue;
		scopeSchema.push_back(*it);
	}
	return scopeSchema;
}

std::list<std::string> Project::GetNamesByScopeAndType(const std::string & scope, const std::string & type)
{
	const std::list<CLAM_Annotator::SchemaAttribute>& hlds = mSchema.GetAllAttributes();
	std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it;
	std::list<std::string> nameList;
	for(it = hlds.begin(); it != hlds.end(); it++)
	{
		if (it->GetScope()!=scope) continue;
		if (it->GetType()!=type) continue;
		nameList.push_back(it->GetName());
	}
	return nameList;
}

}

