#include "MelSpectrum.hxx"


namespace CLAM
{
  
  void MelSpectrum::DefaultInit()
  {
    AddAll();
    UpdateData();
    
    SetSpectralRange(0);
    SetCenterTime(0);
    SetLowCutoff(0);
    SetHighCutoff(0);
    SetNumBands(0);
  }
  
}
