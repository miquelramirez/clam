
#include "ProcessingData.hxx"
#include "TimeIndex.hxx"


namespace CLAM {

  void TimeIndex::DefaultInit()
  {
    AddPosition();
    AddWeight();
    UpdateData();
  }
}
