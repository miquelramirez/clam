#ifndef __MELCEPSTRUM__
#define __MELCEPSTRUM__

#include "ProcessingData.hxx"
#include "Array.hxx"

namespace CLAM
{
  
  class MelCepstrum : public ProcessingData
  {
  public:
    DYNAMIC_TYPE_USING_INTERFACE( MelCepstrum, 4, ProcessingData );
    
    /** Center Time **/
    DYN_ATTRIBUTE( 0, public, TTime, CenterTime );
    /** Low frequency cutoff ( Hz ) **/
    DYN_ATTRIBUTE( 1, public, TData, LowCutoff );
    /** High frequency cutoff ( Hz ) **/
    DYN_ATTRIBUTE( 2, public, TData, HighCutoff );
    /** The cepstrum coefficients **/
    DYN_ATTRIBUTE( 3, public, DataArray, Coefficients );
    
  protected:
    
    void DefaultInit();
    
  };
		
}

#endif // MelCepstrum.hxx
