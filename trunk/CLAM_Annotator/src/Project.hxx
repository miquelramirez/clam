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
	bool LoadScheme(const std::string & schemeFileName, const std::string & basePath="");
	CLAM_Annotator::Schema & GetAnnotatorSchema()
	{
		return mSchema;
	}
	typedef std::list<CLAM_Annotator::SchemaAttribute> ScopeSchema;
	ScopeSchema GetScopeSchema(const std::string & scope) const;
	std::list<std::string> GetNamesByScopeAndType(const std::string & scope, const std::string & type);
	bool ValidateDataPool(const CLAM::DescriptionDataPool & dataPool)
	{
		return GetAnnotatorSchema().Validate(dataPool);
	}
	const CLAM_Annotator::SchemaAttribute & GetAttributeScheme(const std::string & scope, const std::string & name) const
	{
		return mSchema.GetAttribute(scope, name);
	}
private:
	void CreatePoolScheme();
	CLAM::DescriptionScheme mDescriptionScheme;
	CLAM_Annotator::Schema mSchema;
};

}

#endif//_Project_hxx_
