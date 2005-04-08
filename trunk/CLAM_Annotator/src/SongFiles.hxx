
#ifndef _SongFiles_
#define _SongFiles_

#include "DynamicType.hxx"

namespace CLAM_Annotator{

class SongFiles: public CLAM::DynamicType
{
    DYNAMIC_TYPE(SongFiles,1);
    DYN_CONTAINER_ATTRIBUTE(0,public, std::vector<std::string>, FileNames,Name);

  void DefaultInit()
  {
    AddAll();
    UpdateData();
  }
};

};

#endif
