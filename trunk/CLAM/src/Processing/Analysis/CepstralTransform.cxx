#include "CepstralTransform.hxx"
#include "OSDefines.hxx"
#include "Assert.hxx"

namespace CLAM
{
		
  void CepstralTransformConfig::DefaultInit()
  {
    AddAll();
    UpdateData();

    SetUseBase10(false);
  }
  
  CepstralTransform::CepstralTransform(  )
  {
    Configure(CepstralTransformConfig());
  }
  
  CepstralTransform::CepstralTransform( const CepstralTransformConfig& cfg )
  {
    Configure( cfg );
  }
  
  CepstralTransform::~CepstralTransform()
  {
    
  }
  
  bool CepstralTransform::ConcreteConfigure( const ProcessingConfig& cfg )
  {
    CopyAsConcreteConfig( mConfig, cfg );
    
    return true;
  }
  
  bool CepstralTransform::Do()
  {
    return true;
  }
  
  bool CepstralTransform::ConcreteStart()
  {
    mLogBuffer.Resize(mConfig.GetNumMelCoefficients());
    mLogBuffer.SetSize(mConfig.GetNumMelCoefficients());
    
    return true;
  }
  
  bool CepstralTransform::ConcreteStop()
  {
    return true;
  }
  
  bool CepstralTransform::Do( const MelSpectrum& melSpec, MelCepstrum& melCeps )
  {
    if ( !AbleToExecute() ) return false; // object was disabled
    
    const TSize numMelCoefficients = mConfig.GetNumMelCoefficients();
    const TSize numCepstrumCoeffs  = mConfig.GetNumCepstrumCoefficients();

    const DataArray& melCoeffs = melSpec.GetCoefficients();
    DataArray&  cepstrumCoeffs = melCeps.GetCoefficients();

    CLAM_ASSERT( melCoeffs.Size() == numMelCoefficients,
		 "Mel coefficients don't have the expected size!" );
    
    if ( cepstrumCoeffs.Size() < numCepstrumCoeffs ) {
      cepstrumCoeffs.Resize(numCepstrumCoeffs);
      cepstrumCoeffs.SetSize(numCepstrumCoeffs);
    }

    melCeps.SetCenterTime(melSpec.GetCenterTime());
    melCeps.SetLowCutoff(melSpec.GetLowCutoff());
    melCeps.SetHighCutoff(melSpec.GetHighCutoff());

    /* Take the logarithm of the Mel spectrum.  NOTE: What is the
       significance of log vs. log10? */
    const TData melFloor  = 1.0;
    const bool  useBase10 = mConfig.GetUseBase10();

    for (TIndex i=0; i<numMelCoefficients; i++) {
      if (melCoeffs[i] < melFloor)
	mLogBuffer[i] = useBase10 ? log10(melFloor) : log(melFloor);
      else
	mLogBuffer[i] = useBase10 ? log10(melCoeffs[i]) : log(melCoeffs[i]);
    }

    /* Take the DCT of the logarithm. */
    const TData piord = PI / (TData)numMelCoefficients;
    const TData scale = sqrt(2.0/(TData)numMelCoefficients);

    // Compute N-1 last coefficients.
    TData freq = 0;
    for (TIndex i=1; i<numCepstrumCoeffs; i++) {

      freq = i*piord;

      cepstrumCoeffs[i] = 0.0;

      for (TIndex j=0; j<numMelCoefficients; j++) {
	cepstrumCoeffs[i] += mLogBuffer[j] * cos(freq*(j+1 - 0.5));
      }
      cepstrumCoeffs[i] *= scale;
    }
    
    // Compute first coefficient.
    for (TIndex j=0; j<numMelCoefficients; j++) {
      cepstrumCoeffs[0] += mLogBuffer[j];
    }
    cepstrumCoeffs[0] *= scale;
    

    /* NOTE: How about implementing cepstral liftering from HTK? Probably not. */

    return true;
  }
  
}
