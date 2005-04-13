
#ifndef _SongFiles_
#define _SongFiles_

#include "DynamicType.hxx"

namespace CLAM_Annotator{

  class Song:public CLAM::DynamicType
  {
    DYNAMIC_TYPE(Song,2);
    DYN_ATTRIBUTE(0,public, std::string, SoundFile);
    /** Descriptor File Associated to the sound file. In most applications if this attribute
	is missint the application is expected to add the .pool extension to the SoundFile name
    */
    DYN_ATTRIBUTE(1,public, std::string, PoolFile);
    
    void DefaultInit()
    {
      AddSoundFile();
      UpdateData();
    }
  };

  class SongFiles: public CLAM::DynamicType
  {
    DYNAMIC_TYPE(SongFiles,1);
    DYN_CONTAINER_ATTRIBUTE(0,public, std::vector<Song>, FileNames,Name);
    
    void DefaultInit()
    {
      AddAll();
      UpdateData();
    }
  };

};

#endif
