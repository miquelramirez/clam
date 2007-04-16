/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __CEPSTRALTRANSFORM__
#define __CEPSTRALTRANSFORM__

#include "Processing.hxx"
#include "MelSpectrum.hxx"
#include "MelCepstrum.hxx"
#include "Array.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"

namespace CLAM
{

  class CepstralTransformConfig : public ProcessingConfig
  {
  public:
    
    DYNAMIC_TYPE_USING_INTERFACE( CepstralTransformConfig, 3, ProcessingConfig );
    
    /** Number of input samples **/
    DYN_ATTRIBUTE( 0, public, TSize, NumMelCoefficients );
    /** Number of output samples **/
    DYN_ATTRIBUTE( 1, public, TSize, NumCepstrumCoefficients );
    /** Use base 10 or natural logarithm for computation (false by default) **/
    DYN_ATTRIBUTE( 2, public, bool,  UseBase10 );
    
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
    
  private:
    CepstralTransformConfig mConfig;
    DataArray               mLogBuffer;
    InPort<MelSpectrum> mIn;
	OutPort<MelCepstrum> mOut;
    
  };
  
}
#endif // CepstralTransform.hxx

