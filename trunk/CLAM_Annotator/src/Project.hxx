#ifndef _Project_
#define _Project_

#include "DynamicType.hxx"


class Project:public CLAM::DynamicType
{
  DYNAMIC_TYPE(Project,4);
  DYN_ATTRIBUTE(0,public, std::string, Songs);
  DYN_ATTRIBUTE(1,public, std::string, LLDSchema);
  DYN_ATTRIBUTE(2,public, std::string, DescriptorPool);
  DYN_ATTRIBUTE(3,public, std::string, OtherDescriptors);

  void DefaultInit()
  {
    AddAll();
    UpdateData();
  }
};

#endif
