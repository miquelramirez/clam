#ifndef __MELFILTERBANK__
#define __MELFILTERBANK__

#include <Processing.hxx>
#include <Spectrum.hxx>
#include "MelSpectrum.hxx"
#include "Array.hxx"

namespace CLAM
{
  
  class MelFilterBankConfig : public ProcessingConfig
  {
  public:
    DYNAMIC_TYPE_USING_INTERFACE( MelFilterBankConfig, 7, ProcessingConfig );
    
    /** Name of the Processing Object **/
    DYN_ATTRIBUTE( 0, public, std::string, Name );
    /** Spectrum size **/
    DYN_ATTRIBUTE( 1, public, TSize,       SpectrumSize );
    /** Spectral Range **/
    DYN_ATTRIBUTE( 2, public, TData,       SpectralRange );
    /** Low cutoff frequency **/
    DYN_ATTRIBUTE( 3, public, TData,       LowCutoff );
    /** High cutoff frequency **/
    DYN_ATTRIBUTE( 4, public, TData,       HighCutoff );
    /** Number of Bands **/
    DYN_ATTRIBUTE( 5, public, TSize,       NumBands );
    /** Use square of the spectrum for computation (false by default) **/
    DYN_ATTRIBUTE( 6, public, bool,        UsePower );
    
  protected:
    void DefaultInit();
  };
  
  /**
   * This class performs filterbank analysis for Mel Frequency
   * Cepstrum Coefficients (MFCC) computation. It is designed to be
   * compliant with similar functionality found in the speech
   * processing software HTK v3.2.1.
   *
   * No normalization or windowing of the spectrum data is performed
   * prior to analysis.
   *
   * @see CLAM::CepstralTransform
   */
  class MelFilterBank : public Processing
  {
  public:
    MelFilterBank();
    
    MelFilterBank( const MelFilterBankConfig& );
    
    ~MelFilterBank();
    
    bool Do();
    
    /**
     * Perform the filterbank analysis using the specified number of
     * bands.
     *
     * @param s  Input spectrum from FFT transform
     * @param ms Output filterbank coefficients
     *
     * @return True is computation was successfull, false otherwise
     */
    bool Do( Spectrum& s, MelSpectrum& ms );
    
    const ProcessingConfig& GetConfig() const
    {
      return mConfig;
    }
    
    virtual const char* GetClassName() const
    {
      return "MelFilterBank";
    }
    
  protected:
    
    virtual bool ConcreteConfigure( const ProcessingConfig& cfg );
    
    virtual bool ConcreteStart();
    
    virtual bool ConcreteStop();
    
  protected:
    
    /**
     * Linear to Mel scale transformation.
     *
     * @param linFreq Frequency in Hz.
     * @return Frequency in  Mel.
     */
    TData Mel( TData linFreq );

    void InitializeTables();

  private:
    
    MelFilterBankConfig mConfig;
    Array<TIndex>       mMelBand;
    DataArray           mFilterWeights;
    TIndex              mLowIdx;
    TIndex              mHighIdx;
  };
  
}


#endif // MelFilterBank.hxx
