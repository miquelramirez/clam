
#ifndef _SongFiles_
#define _SongFiles_

#include "DynamicType.hxx"

namespace CLAM_Annotator{

	class Song:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Song,2);
		DYN_ATTRIBUTE(0,public, std::string, SoundFile);
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
