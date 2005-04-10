#ifndef _HLDSchema_
#define _HLDSchema_

#include "DynamicType.hxx"

#include <list>
#include <string>

#include "Pool.hxx"
#include "Assert.hxx"

#include "EType.hxx"
#include "Descriptor.hxx"
#include "RestrictedString.hxx"
#include "Range.hxx"

namespace CLAM_Annotator{
  
  //TODO: type should be an enum not a string
  
  class HLDSchemaElement:public CLAM::DynamicType
  {
    DYNAMIC_TYPE(HLDSchemaElement,5);
    DYN_ATTRIBUTE(0, public, std::string, Name);
    //TODO: this should better be an enum
    DYN_ATTRIBUTE(1, public, std::string, Type);
    /*In case type is a RestrictedString, an optional list of enumeration values must be offered*/
    DYN_CONTAINER_ATTRIBUTE(2, public, std::list<std::string>, RestrictionValues, Restriction);
    //In case type is a number we need to add a range
    DYN_ATTRIBUTE(3, public, Range<float>, fRange);
    DYN_ATTRIBUTE(4, public, Range<int>, iRange);
    
    void DefaultInit();
    
  public:
    template <class T>
    bool Validate (const Descriptor<T>& descriptor)const
    {
      return true;
    }
    bool Validate (const Descriptor<CLAM_Annotator::RestrictedString>& descriptor)const;
    bool Validate (const Descriptor<int>& descriptor)const;
    bool Validate (const Descriptor<float>& descriptor)const;
    
  };
    
  
  class HLDSchema:public CLAM::DynamicType
  {
    DYNAMIC_TYPE(HLDSchema,1);
    DYN_CONTAINER_ATTRIBUTE(0,public, std::list<HLDSchemaElement>,HLDs,HLD);
    
    void DefaultInit()
    {
      AddAll();
      UpdateData();
    }
  public:
    template <class T>
    bool ValidateDescriptor(T value,const std::string& name)
    {
      return ValidateDescriptor<T>(MakeDescriptor(value,name));
    }

    template <class T>
    bool ValidateDescriptor(const Descriptor<T>& descriptor)
    {
      bool validated = true;
      
      validated = FindElement(descriptor.GetName()).Validate(descriptor);
      CLAM_DEBUG_ASSERT(validated, std::string("Descriptor did not validate: " + 
						   descriptor.GetName()).c_str());
      return validated;
    }

    bool Validate(const CLAM::DescriptionDataPool& pool);
    HLDSchemaElement FindElement(const std::string& descriptorName) const;
    
  private:
    template <class T>
    Descriptor<T> MakeDescriptor(T value,const std::string& name)
    {
      Descriptor<T> desc;
      desc.SetValue(value);
      desc.SetName(name);
      desc.SetType(GetTypeFromValue(value));
      return desc;
    }
  };


  std::string GetTypeFromValue(int i);
  std::string GetTypeFromValue(float f);
  std::string GetTypeFromValue(const std::string& s);
  std::string GetTypeFromValue(const CLAM_Annotator::RestrictedString& r);
};

#endif
