#include "StationaryResidualSynthesis.hxx"

namespace SALTO
{
	void StationaryResidualSynthesisConfig::DefaultInit()
	{
		AddAll();
		UpdateData();

		SetName( "Stationary Residual Processor" );
	}

	StationaryResidualSynthesis::StationaryResidualSynthesis()
		: mStatResGain( 0 ), mResonanceFreq( 0 ),
		  mInStatResGain( "Statinary Residual Gain Control", this, &StationaryResidualSynthesis::StatResGainCB ),
		  mInResonanceFreq( "Resonance Frequency Control", this, &StationaryResidualSynthesis::ResonanceFreqCB )
	{
		StationaryResidualSynthesisConfig cfg;

		Configure( cfg );
	}

	StationaryResidualSynthesis::StationaryResidualSynthesis( StationaryResidualSynthesisConfig& cfg )
		: mStatResGain( 0 ), mResonanceFreq( 0 ),
		  mInStatResGain( "Statinary Residual Gain Control", this, &StationaryResidualSynthesis::StatResGainCB ),
		  mInResonanceFreq( "Resonance Frequency Control", this, &StationaryResidualSynthesis::ResonanceFreqCB )
	{
		Configure( cfg );
	}

	StationaryResidualSynthesis::~StationaryResidualSynthesis()
	{
	}

	bool StationaryResidualSynthesis::ConcreteStart()
	{
		return true;
	}

	bool StationaryResidualSynthesis::ConcreteStop()
	{
		return true;
	}

	bool StationaryResidualSynthesis::ConcreteConfigure( const ProcessingConfig& c )
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	int StationaryResidualSynthesis::StatResGainCB( TControlData value )
	{
		mStatResGain = value;

		return 0;
	}

	int StationaryResidualSynthesis::ResonanceFreqCB( TControlData value )
	{
		mResonanceFreq = value;

		return 0;
	}

	void StationaryResidualSynthesis::DoStationaryResidualSynthesis(CSaltoSynthFrame  &synthFrame,
	                              Frame *pStatResFrame,
								  TData            statResGain,
								  TData            resonanceFreq)
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

	bool StationaryResidualSynthesis::Do( CSaltoSynthFrame& synthFrame, Frame& pStatResFrame )
	{
//		CLAM_DEBUG_ASSERT( pStatResFrame != NULL, "Check the caller. pStatResFrame MUST NOT be null" );
	
		
		// CAUTION: Hardwired sampling rate value!!!!
		static const double invSamplingRate = 1.0 / 22050.0;
		

		Spectrum *pCurrSpec = synthFrame.GetSpectrumPtr();
		Spectrum *pCurrResidual = synthFrame.GetResidualPtr();
		TSize    specSize = pCurrSpec->GetSize();
		
		Spectrum *pCurrStatResidual = &pStatResFrame.GetResidualSpec();
		// calculate resonance bin width
		TSize centerBin = mResonanceFreq*(specSize-1)*invSamplingRate;
		
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
				(*pCurrCmplx++)+=(*pResCmplx++)*mStatResGain;
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
						(*pCurrCmplx++)+=(*pResCmplx++)*mStatResGain*resonanceCorrection;
						accum+=invPIbinDistance;
					} 
				while ( --counter );
				
			}


		counter = specSize - 1  - rightBin;
		
		
		if (counter)
			{
				do 
					{
						(*pCurrCmplx++)+=(*pResCmplx++)*mStatResGain;
					}
				while (--counter);
			}
	

		return true;
	}

	bool StationaryResidualSynthesis::Do()
	{
		return true;
	}

};
