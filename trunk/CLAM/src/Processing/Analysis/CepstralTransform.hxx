#ifndef __CEPSTRALTRANSFORM__
#define __CEPSTRALTRANSFORM__

#include "Processing.hxx"
#include "MelSpectrum.hxx"
#include "MelCepstrum.hxx"
#include "Array.hxx"

namespace CLAM
{

  class CepstralTransformConfig : public ProcessingConfig
  {
  public:
    
    DYNAMIC_TYPE_USING_INTERFACE( CepstralTransformConfig, 4, ProcessingConfig );
    
    /** Name of the Processing Object**/
    DYN_ATTRIBUTE( 0, public, std::string, Name );
    /** Number of input samples **/
    DYN_ATTRIBUTE( 1, public, TSize,       NumMelCoefficients );
    /** Number of output samples **/
    DYN_ATTRIBUTE( 2, public, TSize,       NumCepstrumCoefficients );
    /** Use base 10 or natural logarithm for computation (false by default) **/
    DYN_ATTRIBUTE( 3, public, bool,        UseBase10 );
    
  protected:
    void DefaultInit();
  };
  
  /**
   * This class performs the cepstral transform on a set of filterbank
   * coefficients in order to obtain the corresponding Mel Frequency
   * Cepstral Coefficients (MFCC). It is designed to be compliant with
   * similar functionality found in the speech processing software HTK
   * v3.2.1.
   *
   * No liftering of the coefficients is performed after transformation.
   *
   * @see CLAM::MelFilterBank
   */
  class CepstralTransform : public Processing
  {
  public:
    CepstralTransform( );
    
    CepstralTransform( const CepstralTransformConfig& cfg );
    
    ~CepstralTransform();
    
    bool Do();
    
    /**
     * Performs the cepstral transform producing the specified number
     * of MFCC coefficients.
     *
     * @param melCoeffs  Input filterbank coefficients
     * @param cepsCoeffs Output MFCC coefficients
     *
     * @return True is computation was successfull, false otherwise
     */
    bool Do( const MelSpectrum& melCoeffs, MelCepstrum& cepsCoeffs );
    
    const ProcessingConfig& GetConfig() const
    {
      return mConfig;
    }
    
    const char* GetClassName() const
    {
      return "CepstralTransform";
    }
    
    
  protected:
    
    bool ConcreteConfigure( const ProcessingConfig& cfg );
    bool ConcreteStart();
    bool ConcreteStop();
    
  private:
    CepstralTransformConfig mConfig;
    DataArray               mLogBuffer;
    
  };
  
}
#endif // CepstralTransform.hxx
