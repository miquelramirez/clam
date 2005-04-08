#ifndef _LLDSchema_
#define _LLDSchema_

#include "DynamicType.hxx"

#include <list>
#include <string>

namespace CLAM_Annotator{

  class LLDSchema:public CLAM::DynamicType
  {
    DYNAMIC_TYPE(LLDSchema,1);
    DYN_CONTAINER_ATTRIBUTE(0,public, std::list<std::string>,LLDNames,Names);
    
    void DefaultInit()
    {
      AddAll();
      UpdateData();
    }
  };
};

#endif
