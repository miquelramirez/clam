#include "MelCepstrum.hxx"

namespace CLAM
{
  void MelCepstrum::DefaultInit()
  {
    AddAll();
    UpdateData();

    SetCenterTime(0);
    SetLowCutoff(0);
    SetHighCutoff(0);
  }
}
