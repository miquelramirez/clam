#ifndef _Project_hxx_
#define _Project_hxx_

#include "Schema.hxx"
#include <CLAM/DynamicType.hxx>
#include "SongFiles.hxx"
#include <CLAM/Filename.hxx>
#include <CLAM/DescriptionScheme.hxx>
#include <vector>
#include <QtCore/QDir>

namespace CLAM_Annotator
{
class Schema;

class Project : public CLAM::DynamicType
{
	DYNAMIC_TYPE(Project,5);
	DYN_ATTRIBUTE(0, public, CLAM::Text, Description);
	DYN_ATTRIBUTE(1, public, CLAM::Filename, Schema);
	DYN_ATTRIBUTE(2, public, CLAM::Filename, Extractor);
	DYN_ATTRIBUTE(3, public, std::string, PoolSuffix);
	DYN_CONTAINER_ATTRIBUTE(4,public, std::vector<Song>, Songs, Song);

	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
public:
	typedef std::list<CLAM_Annotator::SchemaAttribute> ScopeSchema;
public:
	const std::string & PoolSuffix() const
	{
		static std::string defaultSuffix = ".pool";
		if (!HasPoolSuffix()) return defaultSuffix;
		if (GetPoolSuffix()=="") return defaultSuffix;
		return GetPoolSuffix();
	}
	std::string GetDescriptorsFileName(const std::string & songName)
	{
		std::vector<CLAM_Annotator::Song> fileNames = GetSongs();
		std::vector<CLAM_Annotator::Song>::iterator it = fileNames.begin();
		for (int i=0 ; it != fileNames.end(); it++, i++)
		{
			CLAM_Annotator::Song & song = *it;
			if (song.GetSoundFile() != songName) continue;
			if (song.HasPoolFile())
				return song.GetPoolFile();
			else
				return song.GetSoundFile() + PoolSuffix();
		}
		return ""; // BadName
	}
	void AppendSong(const std::string & songFileName)
	{
		std::vector<Song> & songs = GetSongs();
		songs.push_back(Song());
		songs.back().SetSoundFile(songFileName);
	}
	void RemoveSong(const std::string & songFileName)
	{
		std::cout << "Removing '"<<  songFileName << "'" << std::endl;
		std::vector<Song> & songs = GetSongs();
		for (std::vector<Song>::iterator it= songs.begin();
				it!= songs.end(); it++)
		{
			if (it->GetSoundFile() != songFileName) continue;
			std::cout << "Found to remove '"<<  it->GetSoundFile() << "'" << std::endl;
			songs.erase(it);
			return;
		}
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
	void SetProjectPath(const std::string & path)
	{
		mFile = path;
		QString projectPath = QDir::cleanPath((path+"/../").c_str());
		mBasePath = projectPath.toStdString();
	}
	const std::string & File()
	{
		return mFile;
	}
	const std::string & BaseDir()
	{
		return mBasePath;
	}
	std::string ProjectRelativeToAbsolutePath(const std::string & file)
	{
/*
		QDir projectPath(mBasePath.c_str());
		return projectPath.relativeFilePath(file.c_str()).toStdString();
*/
		QDir qdir = QString(file.c_str());
		if (qdir.isRelative())
			return QDir::cleanPath( QDir(mBasePath.c_str()).filePath(file.c_str()) ).toStdString();
		return file;
	}
	std::string AbsoluteToProjectRelativePath(const std::string & file)
	{
		QDir projectPath(mBasePath.c_str());
		return projectPath.relativeFilePath(file.c_str()).toStdString();

		QDir qdir = QString(file.c_str());
		if (qdir.isRelative()) return file;
		if (file.substr(0,mBasePath.length()+1)==(mBasePath+"/"))
			return file.substr(mBasePath.length()+1);
		return file;
	}
private:
	void CreatePoolScheme();
private:
	CLAM::DescriptionScheme mDescriptionScheme;
	CLAM_Annotator::Schema mSchema;
	std::string mFile;
	std::string mBasePath;
};

}

#endif//_Project_hxx_
