#include "Project.hxx"
#include "Text.hxx"
#include "RestrictedString.hxx"
#include "IndexArray.hxx"
#include "XMLStorage.hxx"


namespace CLAM_Annotator
{

void Project::CreatePoolScheme()
{
	//First we start with HLD
	std::list<CLAM_Annotator::HLDSchemaElement>& hlds = mSchema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
	for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
	{
		if((*it2).GetType()=="Float")
		{
			mDescriptionScheme.AddAttribute <float>("Song",(*it2).GetName());
		}
		else if((*it2).GetType()=="Int")
		{
			mDescriptionScheme.AddAttribute <int>("Song",(*it2).GetName());
		}
		else if((*it2).GetType()=="RestrictedString")
		{
			mDescriptionScheme.AddAttribute <CLAM_Annotator::RestrictedString>("Song",(*it2).GetName());
		}
		else
		{
			mDescriptionScheme.AddAttribute <CLAM::Text>("Song",(*it2).GetName());
		}
	}
	//And now we go into LLD
	std::list<std::string>::iterator it;
	std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();
	for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
	{
		mDescriptionScheme.AddAttribute <CLAM::TData>("Frame", (*it));
	}
	//finally we add segmentation marks
	mDescriptionScheme.AddAttribute<CLAM::IndexArray>("Song","Segments");

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

const Project::SongScopeSchema & Project::GetSongScopeSchema()
{
	return GetAnnotatorSchema().GetHLDSchema().GetHLDs();
}
const std::list<std::string> & Project::GetFrameScopeAttributeNames()
{
	return GetAnnotatorSchema().GetLLDSchema().GetLLDNames();
}

	
}

