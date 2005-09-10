#ifndef _Project_hxx_
#define _Project_hxx_

#include "Schema.hxx"
#include "DynamicType.hxx"
#include "SongFiles.hxx"
#include "DescriptionScheme.hxx"
#include <vector>

namespace CLAM_Annotator
{
class Schema;

class Project : public CLAM::DynamicType
{
	DYNAMIC_TYPE(Project,2);
	DYN_ATTRIBUTE(0, public, std::string, Schema);
	DYN_CONTAINER_ATTRIBUTE(1,public, std::vector<Song>, Songs, Song);

	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
public:
	void AppendSong(const std::string & songFileName)
	{
		std::vector<Song> & songs = GetSongs();
		songs.push_back(Song());
		songs.back().SetSoundFile(songFileName);
	}
	CLAM::DescriptionScheme & GetDescriptionScheme()
	{
		return mDescriptionScheme;
	}
	bool LoadScheme(const std::string & schemeFileName);
	CLAM_Annotator::Schema & GetAnnotatorSchema()
	{
		return mSchema;
	}
	typedef std::list<CLAM_Annotator::HLDSchemaElement> SongScopeSchema;
	SongScopeSchema GetSongScopeSchema()
	{
		return GetScopeSchema("Song");
	}
	SongScopeSchema GetScopeSchema(const std::string & scope) const;
	const std::list<std::string> & GetFrameScopeAttributeNames();
	const std::list<std::string> & GetSongSegmentationNames();
	bool ValidateDataPool(const CLAM::DescriptionDataPool & dataPool)
	{
		return GetAnnotatorSchema().GetHLDSchema().Validate(dataPool);
	}
	const CLAM_Annotator::HLDSchemaElement & GetAttributeScheme(const std::string & scope, const std::string & name) const
	{
		std::list<CLAM_Annotator::HLDSchemaElement>& hlds = mSchema.GetHLDSchema().GetHLDs();
		std::list<CLAM_Annotator::HLDSchemaElement>::iterator it;
		for(it = hlds.begin(); it != hlds.end(); it++)
		{
			if (it->GetScope() != scope) continue;
			if (it->GetName() != name) continue;
			return *it;
		}

		std::string message = "Accessing an attribute '";
		message += scope + "':'" + name + "' not in the scheme.";
		CLAM_ASSERT(false, message.c_str());
	}
private:
	void CreatePoolScheme();
	CLAM::DescriptionScheme mDescriptionScheme;
	CLAM_Annotator::Schema mSchema;
	std::list<std::string> mSongSegmentationNames;
};

}

#endif//_Project_hxx_
