
#ifndef _SongFiles_
#define _SongFiles_

#include <CLAM/DynamicType.hxx>
#include <CLAM/Filename.hxx>

namespace CLAM_Annotator{

	class Song:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Song,2);
		DYN_ATTRIBUTE(0,public, CLAM::Filename, SoundFile);
		/**
		 * Descriptor File Associated to the sound file. In most applications if this attribute
		 * is missing the application is expected to add the .pool extension to the SoundFile name
		 */
		DYN_ATTRIBUTE(1,public, std::string, PoolFile);
		
		void DefaultInit()
		{
			AddSoundFile();
			UpdateData();
		}
	};
};

#endif
