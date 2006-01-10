
#ifndef _Range_
#define _Range_

#include <CLAM/Component.hxx>
#include <CLAM/XMLAdapter.hxx>

namespace CLAM_Annotator{

  template <class T> class Range:public CLAM::Component
  {
  public:
    T GetMax() const{return mMax;}
    T GetMin() const{return mMin;}
    void SetMax(T max){mMax = max;}
    void SetMin(T min){mMin = min;}
  private:
    T mMax;
    T mMin;
  public:
    void LoadFrom(CLAM::Storage& storage)
    {
      CLAM::XMLAdapter<T> xmlMin(mMin, "Min", true);
      storage.Load(xmlMin);
      CLAM::XMLAdapter<T> xmlMax(mMax, "Max", true);
      storage.Load(xmlMax);
    }

    void StoreOn(CLAM::Storage& storage)const
    {
      CLAM::XMLAdapter<T> xmlMin(mMin, "Min", true);
      storage.Store(xmlMin);
      CLAM::XMLAdapter<T> xmlMax(mMax, "Max", true);
      storage.Store(xmlMax);
    }

    const char* GetClassName() const {return "Range";}

  };

};

#endif
