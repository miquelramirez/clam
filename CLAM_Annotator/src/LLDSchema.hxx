#ifndef _LLDSchema_
#define _LLDSchema_

#include "DynamicType.hxx"

#include <list>
#include <string>

namespace CLAM{

  class LLDSchema:public DynamicType
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
