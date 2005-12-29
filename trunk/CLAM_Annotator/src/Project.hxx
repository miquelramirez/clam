#ifndef _Project_hxx_
#define _Project_hxx_

#include "Schema.hxx"
#include "DynamicType.hxx"
#include "SongFiles.hxx"
#include "Filename.hxx"
#include "DescriptionScheme.hxx"
#include <vector>

namespace CLAM_Annotator
{
class Schema;

class Project : public CLAM::DynamicType
{
	DYNAMIC_TYPE(Project,3);
	DYN_ATTRIBUTE(0, public, CLAM::Filename, Schema);
	DYN_ATTRIBUTE(1, public, CLAM::Filename, Extractor);
	DYN_CONTAINER_ATTRIBUTE(2,public, std::vector<Song>, Songs, Song);

	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
public:
	typedef std::list<CLAM_Annotator::SchemaAttribute> ScopeSchema;
	void AppendSong(const std::string & songFileName)
	{
		std::vector<Song> & songs = GetSongs();
		songs.push_back(Song());
		songs.back().SetSoundFile(songFileName);
	}
	bool LoadScheme(const std::string & schemeFileName, const std::string & basePath="");
	CLAM::DescriptionScheme & GetDescriptionScheme()
	{
		return mDescriptionScheme;
	}
	CLAM_Annotator::Schema & GetAnnotatorSchema()
	{
		return mSchema;
	}
	ScopeSchema GetScopeSchema(const std::string & scope) const;
	std::list<std::string> GetNamesByScopeAndType(const std::string & scope, const std::string & type);
	bool ValidateDataPool(const CLAM::DescriptionDataPool & dataPool, std::ostream & err)
	{
		return GetAnnotatorSchema().Validate(dataPool,err);
	}
	const CLAM_Annotator::SchemaAttribute & GetAttributeScheme(const std::string & scope, const std::string & name) const
	{
		return mSchema.GetAttribute(scope, name);
	}

	void SetBasePath(const std::string & basePath)
	{
		mBasePath = basePath;
	}
	const std::string & GetBasePath() const
	{
		return mBasePath;
	}
private:
	void CreatePoolScheme();
	CLAM::DescriptionScheme mDescriptionScheme;
	CLAM_Annotator::Schema mSchema;
	std::string mBasePath;
};

}

#endif//_Project_hxx_
