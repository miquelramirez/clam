#ifndef __MELSPECTRUM__
#define __MELSPECTRUM__

#include "ProcessingData.hxx"
#include "Array.hxx"

namespace CLAM
{

  class MelSpectrum : public ProcessingData
  {
  public:
    DYNAMIC_TYPE_USING_INTERFACE( MelSpectrum, 6, ProcessingData );
    
    /** Spectral range **/
    DYN_ATTRIBUTE( 0, public, TData, SpectralRange );
    /** Center time **/
    DYN_ATTRIBUTE( 1, public, TTime, CenterTime );
    /** Low frequency cutoff ( Hz ) **/
    DYN_ATTRIBUTE( 2, public, TData, LowCutoff );
    /** High frequency cutoff ( Hz ) **/
    DYN_ATTRIBUTE( 3, public, TData, HighCutoff );
    /** Number of bands **/
    DYN_ATTRIBUTE( 4, public, TSize, NumBands );
    /** The coefficients **/
    DYN_ATTRIBUTE( 5, public, DataArray, Coefficients );
    
  protected:
    
    void DefaultInit();
  };
  
}

#endif // MelSpectrum.hxx
