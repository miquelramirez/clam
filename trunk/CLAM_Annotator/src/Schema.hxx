#ifndef _Schema_
#define _Schema_

#include "DynamicType.hxx"
#include "HLDSchema.hxx"
#include "LLDSchema.hxx"

namespace CLAM_Annotator{


  class Schema:public CLAM::DynamicType
  {
    DYNAMIC_TYPE(Schema,2);
    DYN_ATTRIBUTE(0,public, HLDSchema, HLDSchema);
    DYN_ATTRIBUTE(1,public, LLDSchema, LLDSchema);

    void DefaultInit()
    {
      AddAll();
      UpdateData();
    }
  };
    
};

#endif
