#include "CSaltoResidualSynthesis.hxx"
#include "HeapDbg.hxx"
#include <iostream>

namespace CLAM
{

CSaltoResidualSynthesis::CSaltoResidualSynthesis(TSize spectralFrameSize, TTime frameTime)
{
}

CSaltoResidualSynthesis::~CSaltoResidualSynthesis()
{
}
//----------------------------------------------------------------------------//

void CSaltoResidualSynthesis::DoAttackResidualSynthesis(CSaltoSynthFrame& synthFrame, TData attackResGain)
{
	Spectrum *pCurrSpec = synthFrame.GetSpectrumPtr();
	Spectrum *pCurrResidual = synthFrame.GetResidualPtr();
	TSize	specSize = pCurrSpec->GetSize();
	
	if (pCurrResidual!=NULL&&pCurrResidual->GetSize()==pCurrSpec->GetSize())  // only if theres a attack residual !
	{
		if ( specSize != 0)
		{
			Complex* pSpecCmplx = pCurrSpec->GetComplexArray().GetPtr();
			Complex* pResCmplx = pCurrResidual->GetComplexArray().GetPtr();

			do
			{
				(*pSpecCmplx++)+=(*pResCmplx++)*attackResGain;
			}
			while ( --specSize );

  			//for (int i=0;i<specSize;i++)
  			//{
//  		  pCurrSpec->Set(i,pCurrSpec->GetComplex(i)+pCurrResidual->GetComplex(i)*attackResGain);
 			// Buena: pCurrSpec->GetComplexArray()[i] = pCurrSpec->GetComplexArray()[i]+(pCurrResidual->GetComplexArray()[i])*attackResGain;
  			//}
		}
	}
}

//----------------------------------------------------------------------------//
void CSaltoResidualSynthesis::DoStatResidualSynthesis
  (CSaltoSynthFrame &synthFrame,
   Frame 			*pStatResFrame,
   TData   			statResGain,
   TData   			resonanceFreq)
{
	
	Spectrum *pCurrSpec = synthFrame.GetSpectrumPtr();
	Spectrum *pCurrResidual = synthFrame.GetResidualPtr();
	TSize    specSize = pCurrSpec->GetSize();
	
	if (pStatResFrame!=NULL)  // only if theres a Stationary residual !
	{
  		Spectrum *pCurrStatResidual = &pStatResFrame->GetResidualSpec();
  		// calculate resonance bin width
  		TSize centerBin = resonanceFreq*(specSize-1)/22050.;
  		TSize leftBin = centerBin - (0.9*resonanceFreq*(specSize-1)/22050.);
  		if (leftBin<=0) leftBin = 1;
  		TSize rightBin = centerBin +(1.1*resonanceFreq*(specSize-1)/22050.);
  		if (rightBin>=specSize) rightBin = specSize-1;
  		double binDistance = rightBin-leftBin;
  		 
  		double resonanceCorrection;
  		 
  		for (int i=0;i<specSize;i++)
  		{
			if(false)//(i>=leftBin&&i<=rightBin)
			{
      			resonanceCorrection = (sin((i-leftBin)*TData(PI)/binDistance)+1)*1;
		//         pCurrSpec->Set(i,pCurrSpec->GetComplex(i)+pCurrStatResidual->GetComplex(i)*statResGain*resonanceCorrection);
  		    	pCurrSpec->GetComplexArray()[i] = pCurrSpec->GetComplexArray()[i]+pCurrStatResidual->GetComplexArray()[i]*statResGain*resonanceCorrection;
			}  		
  			else
			{
		//  		    pCurrSpec->Set(i,pCurrSpec->GetComplex(i)+pCurrStatResidual->GetComplex(i)*statResGain);
  					pCurrSpec->GetComplexArray()[i] = pCurrSpec->GetComplexArray()[i]+pCurrStatResidual->GetComplexArray()[i]*statResGain;
			}
  		}
	}
}

void CSaltoResidualSynthesis::DoStatResidualSynthesis2 ( CSaltoSynthFrame &synthFrame, 
														 Frame *pStatResFrame, 
														 TData statResGain, 
														 TData resonanceFreq)
{
	CLAM_DEBUG_ASSERT( pStatResFrame != NULL, "Check the caller. pStatResFrame MUST NOT be null" );
	
	
	// CAUTION: Hardwired sampling rate value!!!!
	static const double invSamplingRate = 1.0 / 22050.0;
	

	Spectrum *pCurrSpec = synthFrame.GetSpectrumPtr();
	Spectrum *pCurrResidual = synthFrame.GetResidualPtr();
	TSize    specSize = pCurrSpec->GetSize();
	
	Spectrum *pCurrStatResidual = &pStatResFrame->GetResidualSpec();
	// calculate resonance bin width
	TSize centerBin = resonanceFreq*(specSize-1)*invSamplingRate;
	
	TSize leftBin = centerBin - (0.9*centerBin);
	
	if (leftBin<=0) 
		leftBin = 1;
	
	TSize rightBin = centerBin + (1.1*centerBin);
	
	if (rightBin>=specSize) 
		rightBin = specSize-1;
	
	double binDistance = rightBin-leftBin;

	double resonanceCorrection;
	
	Complex* pCurrCmplx = pCurrSpec->GetComplexArray().GetPtr();
	Complex* pResCmplx = pCurrStatResidual->GetComplexArray().GetPtr();
	TSize counter = leftBin;
	//		TSize counter = specSize;

	
	do 
		{
			(*pCurrCmplx++)+=(*pResCmplx++)*statResGain;
		}
	while (--counter);
	
	
	if ( binDistance )
		{
			const double invPIbinDistance = TData(PI)/binDistance;
	
			counter = binDistance+1;
			double accum = 0;
			do 
				{
					resonanceCorrection = sin( accum )+1;
					(*pCurrCmplx++)+=(*pResCmplx++)*statResGain*resonanceCorrection;
					accum+=invPIbinDistance;
				} 
			while ( --counter );
			
		}


	counter = specSize - 1  - rightBin;
	
	
	if (counter)
		{
			do 
				{
					(*pCurrCmplx++)+=(*pResCmplx++)*statResGain;
				}
			while (--counter);
		}
	
	
}


}
