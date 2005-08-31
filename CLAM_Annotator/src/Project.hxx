#ifndef _Project_hxx_
#define _Project_hxx_

#include "DynamicType.hxx"
#include "SongFiles.hxx"
#include <vector>

namespace CLAM_Annotator
{

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
};

}

#endif//_Project_hxx_
