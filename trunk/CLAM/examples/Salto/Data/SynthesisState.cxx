#include "SynthesisState.hxx"

namespace SALTO
{
	void SynthesisState::DefaultInit( void )
	{
		AddAll();
		UpdateData();

		SetInLoopSynthesis( false );
		SetLoopDirectionFW( true );
		SetStatResLoopDirectionFW( true );
		SetFrameCounterTransition( 0.0 );
		SetIndividualGain( 50.0 );
		SetLimitLoopStart( 0 );
		SetLimitLoopEnd( 0 );
		SetLoopStart( 0 );
		SetLoopEnd( 0 );
		SetInterpolStart( 0 );
		SetInterpolEnd( 0 );
		SetResFadeStart( 0 );
		SetResFadeEnd( 0 );
		SetAttackResVolume( 1.0 );
		SetFrameCounterStatRes( 0 );
		SetNumFramesStatRes( 0 );
		SetStatResFadeInFrom( 20 );
		SetStatResFadeInTo( 40 );
		SetStatResVolume( 1.0 );
		SetFrameCounterAttackResidual( 0 );
		SetCurrentStatTemplate( 0 );
		SetIPFactor( 0.0 );
		SetLastPitchCorrectionFactor( 0 );
		SetLastAlignedFrame( false );
		SetTransitionMode( false );
	}

	CSaltoSegData SynthesisState::GetSegDataByPos(TIndex pos)
	{
	  if (pos>=0 && pos <SPECTRAL_SEGMENTS_IN_USE)
		return mSegData[pos];
	  else
		return mSegData[0]; // return no error ! Check when this happens !!!!
		//CLAM_ASSERT(false,"CSaltoDataManagment::GetSegDataByPos out of Bounds ");
	}


	SpectralPeakArray*  SynthesisState::GetCurrentStatTemplatePtr()
	{
	  //  switch (mCurrentStatTemplate)
	  switch (GetCurrentStatTemplate())
	  {
		case 0:
		{
		  return &mCurrentStatTmpl0;
		  break;
		}
		case 1:
		{
		  return &mCurrentStatTmpl1;
		  break;
		}
		case 2:
		{
		  return &mCurrentStatTmpl2;
		  break;
		}
		case 3:
		{
		  return &mCurrentStatTmpl3;
		  break;
		}
		case 4:
		{
		  return &mCurrentStatTmpl4;
		  break;
		}
		case 5:
		{
		  return &mCurrentStatTmpl5;
		  break;
		}
		case 6:
		{
		  return &mCurrentStatTmpl6;
		  break;
		}
		case 7:
		default:
		{
		  return &mCurrentStatTmpl7;
		  break;
		}
	  }
	}
	
	double SynthesisState::GetCurrentStatTemplateFundFreq()
	{
	  switch (GetCurrentStatTemplate())
	  {
		case 0:
		{
		  return mCurrentStatFundFreq0;
		  break;
		}
		case 1:
		{
		  return mCurrentStatFundFreq1;
		  break;
		}
		case 2:
		{
		  return mCurrentStatFundFreq2;
		  break;
		}
		case 3:
		{
		  return mCurrentStatFundFreq3;
		  break;
		}
		case 4:
		{
		  return mCurrentStatFundFreq4;
		  break;
		}
		case 5:
		{
		  return mCurrentStatFundFreq5;
		  break;
		}
		case 6:
		{
		  return mCurrentStatFundFreq6;
		  break;
		}
		case 7:
		default:
		{
		  return mCurrentStatFundFreq7;
		  break;
		}
	  }	
	}
}
