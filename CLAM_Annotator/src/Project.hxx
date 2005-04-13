#ifndef _Project_
#define _Project_

#include "DynamicType.hxx"

namespace CLAM_Annotator{

class Project:public CLAM::DynamicType
{
  DYNAMIC_TYPE(Project,2);
  DYN_ATTRIBUTE(0,public, std::string, Songs);
  DYN_ATTRIBUTE(1,public, std::string, Schema);

  void DefaultInit()
  {
    AddAll();
    UpdateData();
  }
};

};

#endif
