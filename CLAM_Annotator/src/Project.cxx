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
	mSongSegmentationNames.clear();
	//First we start with HLD
	std::list<CLAM_Annotator::HLDSchemaElement>& hlds = mSchema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
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
			mSongSegmentationNames.push_back(name);
			mDescriptionScheme.AddAttribute<CLAM::IndexArray>(scope,name);
		}
	}
	//And now we go into LLD
	std::list<std::string>::iterator it;
	std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();
	for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
	{
		mDescriptionScheme.AddAttribute <CLAM::TData>("Frame", (*it));
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

Project::SongScopeSchema Project::GetScopeSchema(const std::string & scope) const
{
	Project::SongScopeSchema scopeSchema;
	std::list<CLAM_Annotator::HLDSchemaElement>& hlds = mSchema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it;
	for(it = hlds.begin(); it != hlds.end(); it++)
	{
		if (it->GetScope()!=scope) continue;
		if (it->GetType()=="Segmentation") continue;
		scopeSchema.push_back(*it);
	}
	return scopeSchema;
}

const std::list<std::string> & Project::GetFrameScopeAttributeNames()
{
	return GetAnnotatorSchema().GetLLDSchema().GetLLDNames();
}

const std::list<std::string> & Project::GetSongSegmentationNames()
{
	return mSongSegmentationNames;
}

}

