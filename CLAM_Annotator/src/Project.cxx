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
		const std::string & type = it2->GetType();
		const std::string & name = it2->GetName();
		if (type=="Float")
		{
			mDescriptionScheme.AddAttribute <float>("Song",name);
		}
		else if (type=="Int")
		{
			mDescriptionScheme.AddAttribute <int>("Song",name);
		}
		else if (type=="RestrictedString")
		{
			mDescriptionScheme.AddAttribute <CLAM_Annotator::RestrictedString>("Song",name);
		}
		else if (type=="String")
		{
			mDescriptionScheme.AddAttribute <CLAM::Text>("Song",name);
		}
	}
	//And now we go into LLD
	std::list<std::string>::iterator it;
	std::list<std::string>& descriptorsNames = mSchema.GetLLDSchema().GetLLDNames();
	for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
	{
		mDescriptionScheme.AddAttribute <CLAM::TData>("Frame", (*it));
	}
	mDescriptionScheme.AddAttribute<CLAM::IndexArray>("Song","Segments");
	mSongSegmentationNames.clear();
	mSongSegmentationNames.push_back("Segments");
	mSongSegmentationNames.push_back("RandomSegments");
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

const std::list<std::string> & Project::GetSongSegmentationNames()
{
	return mSongSegmentationNames;
}

}

