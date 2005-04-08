
#ifndef _Descriptor_
#define _Descriptor_

#include <string>

namespace CLAM_Annotator{

  template<class T> class Descriptor
  {
  public:
    T GetValue() const{return mValue;}
    std::string GetName() const{return mName;}
    std::string GetType() const{return mType;}
    
    void SetValue(const T& value) {mValue = value;}
    void SetName(const std::string& name) {mName = name;}
    void SetType(const std::string& type) {mType = type;}
  protected:
    T mValue;
    std::string mName;
    std::string mType;
  };

};

#endif
