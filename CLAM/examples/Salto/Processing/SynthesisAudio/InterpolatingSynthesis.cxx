#include "InterpolatingSynthesis.hxx"

namespace SALTO
{
	void InterpolatingSynthesisConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}
	
	InterpolatingSynthesis::InterpolatingSynthesis()
		: mOneTrackAlreadyStarted( false ),mRandomIndex( 0 ), mInLoopSynthesis( false ),
		  mUseRandomDeviations( false ), mUseRandomLoop( false ), mPitchFactor( 0 ), 
		  mRandomRange( 8 ),mTargetFreq( 0 ),mMagInterpolFactor( 0 ), mMagGain( 0 ),
		  mFreqInterpolFactor( 0 ), mMagInterpolFactor2( 0 ),
  		  mIn_InLoopSynthesis( " In Loop Synthesis Control", this, &InterpolatingSynthesis::InLoopSynthesisCB ),
		  mInUseRandomLoop( " Use Random Loop Control", this, &InterpolatingSynthesis::UseRandomLoopCB ),
		  mInUseRandomDeviations( " Use Random Deviations Control", this, &InterpolatingSynthesis::UseRandomDeviationsCB ),
		  mInPitchFactor( "Pitch Factor Control", this, &InterpolatingSynthesis::PitchFactorCB ),
		  mInRandomRange( "Random Range Control", this, &InterpolatingSynthesis::RandomRangeCB ),
		  mInTargetFreq( "Target Frequency Control", this, &InterpolatingSynthesis::TargetFreqCB ),
		  mInMagInterpolFactor( "Magnitude Interpolation Factor" , this, &InterpolatingSynthesis::MagInterpolFactorCB),
		  mInMagGain( "Magnitude Gain", this, &InterpolatingSynthesis::MagGainCB ),
		  mInFreqInterpolFactor( "Frequency Interpolation Factor Control", this, &InterpolatingSynthesis::FreqInterpolFactorCB ),
		  mInMagInterpolFactor2( "Magnitude Interpolation Factor 2 Control", this, &InterpolatingSynthesis::MagInterpolFactor2CB )
	{
		InterpolatingSynthesisConfig cfg;

		Configure( cfg );
	}

	InterpolatingSynthesis::InterpolatingSynthesis( const InterpolatingSynthesisConfig& cfg)
		: mOneTrackAlreadyStarted( false ),mRandomIndex( 0 ), mInLoopSynthesis( false ),
		  mUseRandomDeviations( false ), mUseRandomLoop( false ), mPitchFactor( 0 ),
		  mRandomRange( 8 ),mTargetFreq( 0 ),mMagInterpolFactor( 0 ), mMagGain( 0 ),
		  mFreqInterpolFactor( 0 ), mMagInterpolFactor2( 0 ),
  		  mIn_InLoopSynthesis( " In Loop Synthesis Control", this, &InterpolatingSynthesis::InLoopSynthesisCB ),
		  mInUseRandomLoop( " Use Random Loop Control", this, &InterpolatingSynthesis::UseRandomLoopCB ),
		  mInUseRandomDeviations( " Use Random Deviations Control", this, &InterpolatingSynthesis::UseRandomDeviationsCB ),
		  mInPitchFactor( "Pitch Factor Control", this, &InterpolatingSynthesis::PitchFactorCB ),
		  mInRandomRange( "Random Range Control", this, &InterpolatingSynthesis::RandomRangeCB ),
		  mInTargetFreq( "Target Frequency Control", this, &InterpolatingSynthesis::TargetFreqCB ),
		  mInMagInterpolFactor( "Magnitude Interpolation Factor" , this, &InterpolatingSynthesis::MagInterpolFactorCB),
		  mInMagGain( "Magnitude Gain", this, &InterpolatingSynthesis::MagGainCB ),
		  mInFreqInterpolFactor( "Frequency Interpolation Factor Control", this, &InterpolatingSynthesis::FreqInterpolFactorCB ),
		  mInMagInterpolFactor2( "Magnitude Interpolation Factor 2 Control", this, &InterpolatingSynthesis::MagInterpolFactor2CB )
	{
		Configure( cfg );
	}

	InterpolatingSynthesis::~InterpolatingSynthesis()
	{}

	bool InterpolatingSynthesis::ConcreteConfigure( const ProcessingConfig& c )
	{
		CopyAsConcreteConfig(mConfig, c);

		mSpectralRange = mConfig.GetSpectralRange();

		for (int i = 0; i<MAX_SINES;i++)
		{
			mRandomNumber[i] = (TFloat32)rand()/RAND_MAX;
		}


		return true;
	}

	bool InterpolatingSynthesis::ConcreteStart()
	{
		return true;
	}

	bool InterpolatingSynthesis::ConcreteStop()
	{
		return true;
	}

	int InterpolatingSynthesis::InLoopSynthesisCB( TControlData value )
	{
		if( value > 0 )		mInLoopSynthesis = true;
		else				mInLoopSynthesis = false;

		return 0;
	}

	int InterpolatingSynthesis::UseRandomDeviationsCB( TControlData value )
	{
		if( value > 0 )		mUseRandomDeviations = true;
		else				mUseRandomDeviations = false;

		return 0;
	}

	int InterpolatingSynthesis::UseRandomLoopCB( TControlData value )
	{
		if( value > 0 )		mUseRandomLoop = true;
		else				mUseRandomLoop = false;

		return 0;
	}

	int InterpolatingSynthesis::PitchFactorCB( TControlData value )
	{
		mPitchFactor = value;

		return 0;
	}

	int InterpolatingSynthesis::RandomRangeCB( TControlData value )
	{
		mRandomRange = value;

		return 0;
	}

	int InterpolatingSynthesis::TargetFreqCB( TControlData value )
	{
		mTargetFreq = value;

		return 0;
	}

	int InterpolatingSynthesis::MagInterpolFactorCB( TControlData value )
	{
		mMagInterpolFactor = value;

		return 0;
	}

	int InterpolatingSynthesis::MagGainCB( TControlData value )
	{
		mMagGain = value;
			
		return 0;
	}

	int InterpolatingSynthesis::FreqInterpolFactorCB( TControlData value )
	{
		mFreqInterpolFactor = value;

		return 0;
	}

	int InterpolatingSynthesis::MagInterpolFactor2CB( TControlData value )
	{
		mMagInterpolFactor2 = value;

		return 0;
	}

	inline double InterpolatingSynthesis::DoLinInterpolation(double base,double target,double ipFactor)
	{
	  return base+ipFactor*(target-base);
	}

	void InterpolatingSynthesis::DoMagInterpolation(Frame &pSpecFrameBase,	
												  SpectralPeakArray &pTargetPeakArray,
//												  double targetFundFreq,      
//												  double magInterpolFactor,
//												  double magGain,             
												  CSaltoSynthFrame &pSynthFrame )
	{
		SpectralPeakArray*	pBasePeakArray = &(pSpecFrameBase.GetSpectralPeakArray()); //BUGFRAME!
		double base,target,tempValue;
		
		// this should be elaborated ....
		TSize nPeaks1 = pBasePeakArray->GetnPeaks();
		TSize nPeaks2 = pTargetPeakArray.GetnPeaks();
		
		if ((nPeaks1>=nPeaks2||nPeaks1==0)|| mUseRandomLoop )//(nPeaks1>=nPeaks2||nPeaks1==0)// for now always true.... future:nPeaks1>=nPeaks2 ||pTargetPeakArray2!=NULL
			{
				// Pre: { nPeaks1>=nPeaks2 v nPeaks1==0 v mpParams->GetUseRandomLoop()}
				pSynthFrame.SetPeakArraySize(nPeaks1);
				// Interpolated peak array iterators

				if ( nPeaks1 == 0 ) return;
				
				TData* pkFreqBuffer = pSynthFrame.GetPeakArray().GetFreqBuffer().GetPtr();;
				TData* pkMagBuffer = pSynthFrame.GetPeakArray().GetMagBuffer().GetPtr();
				TData* pkPhaseBuffer = pSynthFrame.GetPeakArray().GetPhaseBuffer().GetPtr();
				TData* pkBinPosBuffer = pSynthFrame.GetPeakArray().GetBinPosBuffer().GetPtr();
				TData* pkBinWidthBuffer = pSynthFrame.GetPeakArray().GetBinWidthBuffer().GetPtr();
				int* pkIndexArrayBuffer = pSynthFrame.GetPeakArray().GetIndexArray().GetPtr();
	
		
				// Base peak array iterators
		//		pBasePeakArray->Debug();
				TData* pBaseFreq = pBasePeakArray->GetFreqBuffer().GetPtr();
				TData* pBasePhase = pBasePeakArray->GetPhaseBuffer().GetPtr();
				TData* pBaseMag = pBasePeakArray->GetMagBuffer().GetPtr();
				int* pBaseIndex = pBasePeakArray->GetIndexArray().GetPtr();
				
				// Target peak array iterators
				TData* pTargetMag = pTargetPeakArray.GetMagBuffer().GetPtr();

				int* pTargetIndex = pTargetPeakArray.GetIndexArray().GetPtr();

				for(int i=0;i<nPeaks1;i++)
					{
						*pkFreqBuffer++ = *pBaseFreq++;
						*pkPhaseBuffer++ = *pBasePhase++;

						
						// lin mag interpolation
						base = *pBaseMag++; 	
						if (i<nPeaks2)
							{	
								target = *pTargetMag++;
							}
						else
							{
								target = 0; // if there is no matching peak , set this track to zero
								pTargetMag++;
							}

						tempValue = (base+mMagInterpolFactor*(target-base));
						

						*pkMagBuffer++ = tempValue*mMagGain;
						*pkBinPosBuffer++ = -1;
						*pkBinWidthBuffer++ = -1;
						
						// use base array indizes for sinusoidal track indexing ....
						//    		pSynthFrame->SetPeakArrayIndex(i,pBasePeakArray->GetIndex(i));
						*pkIndexArrayBuffer++ = *pBaseIndex++;
					}

			}
		else
			{
				// Pre: { nPeaks1<nPeaks2 && nPeaks1!=0 && !mpParams->GetUseRandomLoop()}

				pSynthFrame.SetPeakArraySize(nPeaks2);
				
				// Interpolated peak array iterators
				TData* pkMagBuffer = pSynthFrame.GetPeakArray().GetMagBuffer().GetPtr();
				TData* pkFreqBuffer = pSynthFrame.GetPeakArray().GetFreqBuffer().GetPtr();
				TData* pkPhaseBuffer = pSynthFrame.GetPeakArray().GetPhaseBuffer().GetPtr();
				TData* pkBinPosBuffer = pSynthFrame.GetPeakArray().GetBinPosBuffer().GetPtr();
				TData* pkBinWidthBuffer = pSynthFrame.GetPeakArray().GetBinWidthBuffer().GetPtr();
				int* pkIndexArrayBuffer = pSynthFrame.GetPeakArray().GetIndexArray().GetPtr();
				
				// Base peak array iterators
				TData* pBaseFreq = pBasePeakArray->GetFreqBuffer().GetPtr();
				TData* pBasePhase = pBasePeakArray->GetPhaseBuffer().GetPtr();
				TData* pBaseMag = pBasePeakArray->GetMagBuffer().GetPtr();
				int* pBaseIndex = pBasePeakArray->GetIndexArray().GetPtr();
				
				// Target peak array iterators
				TData* pTargetMag = pTargetPeakArray.GetMagBuffer().GetPtr();


				int* pTargetIndex = pTargetPeakArray.GetIndexArray().GetPtr();
				int i; // For the 'for' loops

				for( i=0;i<nPeaks1;i++)
					{
						// Inv: { 0 < i < nPeaks1 }
						// Cota: {nPeaks1}
						// Inv && (nPeaks1 < nPeaks2) = 0 < i < nPeaks1 < nPeaks2

						*pkFreqBuffer++ = *pBaseFreq++;

						*pkPhaseBuffer++ = *pBasePhase++;

						
						// lin mag interpolation
						base = *pBaseMag++; 	
						target = *pTargetMag++;
						tempValue = (base+mMagInterpolFactor*(target-base));
						

						*pkMagBuffer++ = tempValue*mMagGain;
						*pkBinPosBuffer++ = -1;
						*pkBinWidthBuffer++ = -1;
						
						// use base array indizes for sinusoidal track indexing ....
						//    		pSynthFrame->SetPeakArrayIndex(i,pBasePeakArray->GetIndex(i));
						*pkIndexArrayBuffer++ = *pBaseIndex++;
					}
			// create new partials
				
				TData magInterpolFactor3magGain = mMagInterpolFactor*mMagInterpolFactor*mMagInterpolFactor*mMagGain;
				TData fundFreq = pSpecFrameBase.GetFundamental().GetFreq();
				TData freq = fundFreq*nPeaks1;
				TData* pTargetPhase = &(pTargetPeakArray.GetPhaseBuffer().GetPtr()[nPeaks1]);

				for(i= nPeaks1;i<nPeaks2;i++, freq+=fundFreq )
					{ 
						if (freq>=mSpectralRange||freq<50) // 50 is a preliminary value 
							{
								pSynthFrame.SetPeakArraySize(i);
								break; // dont create new peaks out of range ...
							}
	      				
						*pkFreqBuffer++ = freq;
						*pkPhaseBuffer++ = *pTargetPhase++;
						*pkMagBuffer++ = (*pTargetMag++)*magInterpolFactor3magGain;
						*pkBinPosBuffer++ = -1;
						*pkBinWidthBuffer++ = -1;
						*pkIndexArrayBuffer++ = i;
					}
			}
		
		// fund freq of BaseFrame
		base = pSpecFrameBase.GetFundamental().GetFreq();
		CLAM_ASSERT(base>=0, "base < o!!\n");

		pSynthFrame.SetFundFreq(base);

	
	}

	void InterpolatingSynthesis::DoInterpolation( Frame &pSpecFrameBase,	
												  SpectralPeakArray &pTargetPeakArray,
//												  double targetFundFreq,      
//												  double magInterpolFactor,
//												  double freqInterpolFactor,  
//												  double magGain,             
												  CSaltoSynthFrame &pSynthFrame,
												  SpectralPeakArray &pTargetPeakArray2)
//												  double magInterpolFactor2 )
	{
		SpectralPeakArray*	pBasePeakArray = &pSpecFrameBase.GetSpectralPeakArray(); //BUGFRAME!
		double base,target,tempValue;
		
		// this should be elaborated ....
		TSize nPeaks1 = pBasePeakArray->GetnPeaks();
		TSize nPeaks2 = pTargetPeakArray.GetnPeaks();
		TSize nPeaks3 = pTargetPeakArray2.GetnPeaks();
		
		if ((nPeaks1>=nPeaks2||nPeaks1==0)|| mUseRandomLoop )//(nPeaks1>=nPeaks2||nPeaks1==0)// for now always true.... future:nPeaks1>=nPeaks2 ||pTargetPeakArray2!=NULL
			{
				// Pre: { nPeaks1>=nPeaks2 v nPeaks1==0 v mpParams->GetUseRandomLoop()}
				pSynthFrame.SetPeakArraySize(nPeaks1);
				// Interpolated peak array iterators

				if ( nPeaks1 == 0 ) return;
				
				TData* pkFreqBuffer = pSynthFrame.GetPeakArray().GetFreqBuffer().GetPtr();;
				TData* pkMagBuffer = pSynthFrame.GetPeakArray().GetMagBuffer().GetPtr();
				TData* pkPhaseBuffer = pSynthFrame.GetPeakArray().GetPhaseBuffer().GetPtr();
				TData* pkBinPosBuffer = pSynthFrame.GetPeakArray().GetBinPosBuffer().GetPtr();
				TData* pkBinWidthBuffer = pSynthFrame.GetPeakArray().GetBinWidthBuffer().GetPtr();
				int* pkIndexArrayBuffer = pSynthFrame.GetPeakArray().GetIndexArray().GetPtr();
	
		
				// Base peak array iterators
		//		pBasePeakArray->Debug();
				TData* pBaseFreq = pBasePeakArray->GetFreqBuffer().GetPtr();
				TData* pBasePhase = pBasePeakArray->GetPhaseBuffer().GetPtr();
				TData* pBaseMag = pBasePeakArray->GetMagBuffer().GetPtr();
				int* pBaseIndex = pBasePeakArray->GetIndexArray().GetPtr();
				
				// Target peak array iterators
				TData* pTargetFreq = pTargetPeakArray.GetFreqBuffer().GetPtr();
				TData* pTargetMag = pTargetPeakArray.GetMagBuffer().GetPtr();
				TData* pTargetMag2 = pTargetPeakArray2.GetMagBuffer().GetPtr();

				int* pTargetIndex = pTargetPeakArray.GetIndexArray().GetPtr();

				for(int i=0;i<nPeaks1;i++)
					{
						// lin freq interpolation
						base = *pTargetFreq++;
						if (i<nPeaks2)
							{
								*pkFreqBuffer++ = DoLinInterpolation(*pBaseFreq++,base,mFreqInterpolFactor);
								*pkPhaseBuffer++ = DoLinInterpolation(*pBasePhase++,base,mFreqInterpolFactor);
									
								// lin mag interpolation
								target = *pTargetMag++;
								base = *pBaseMag++; 
								tempValue = (base+mMagInterpolFactor*(target-base));

								if (i<nPeaks3)
									{
										tempValue = tempValue + mMagInterpolFactor2 * ((*pTargetMag2++)-tempValue);
									}
								else
									{
										tempValue = 0; // FADE OUT TRACK...
										pTargetMag2++;
									}
							}
						else
							{
								*pkFreqBuffer++ = *pBaseFreq++;
								*pkPhaseBuffer++ = *pBasePhase++;
								//target = 0; // if there is no matching peak , set this track to zero
								pTargetMag++;

								// lin mag interpolation
								//base = *pBaseMag++; 
								//tempValue = (base+magInterpolFactor*(-base));
								pBaseMag++ ;
								tempValue = 0; // FADE OUT TRACK...
								pTargetMag2++;

							}
						
												
						// lin mag interpolation
					//	base = *pBaseMag++; 
					//	tempValue = (base+magInterpolFactor*(target-base));
						

					/*	if (i<nPeaks3&&i<nPeaks2)
							{
								tempValue = tempValue + magInterpolFactor2 * ((*pTargetMag2++)-tempValue);
							}
						else
							{
								tempValue = 0; // FADE OUT TRACK...
								pTargetMag2++;
							}
					*/

						*pkMagBuffer++ = tempValue*mMagGain;
						*pkBinPosBuffer++ = -1;
						*pkBinWidthBuffer++ = -1;
						
						// use base array indizes for sinusoidal track indexing ....
						//    		pSynthFrame->SetPeakArrayIndex(i,pBasePeakArray->GetIndex(i));
						*pkIndexArrayBuffer++ = *pBaseIndex++;
					}

			}
		else
			{
				// Pre: { nPeaks1<nPeaks2 && nPeaks1!=0 && !mpParams->GetUseRandomLoop()}

				pSynthFrame.SetPeakArraySize(nPeaks2);
				
				// Interpolated peak array iterators
				TData* pkMagBuffer = pSynthFrame.GetPeakArray().GetMagBuffer().GetPtr();
				TData* pkFreqBuffer = pSynthFrame.GetPeakArray().GetFreqBuffer().GetPtr();
				TData* pkPhaseBuffer = pSynthFrame.GetPeakArray().GetPhaseBuffer().GetPtr();
				TData* pkBinPosBuffer = pSynthFrame.GetPeakArray().GetBinPosBuffer().GetPtr();
				TData* pkBinWidthBuffer = pSynthFrame.GetPeakArray().GetBinWidthBuffer().GetPtr();
				int* pkIndexArrayBuffer = pSynthFrame.GetPeakArray().GetIndexArray().GetPtr();
				
				// Base peak array iterators
				TData* pBaseFreq = pBasePeakArray->GetFreqBuffer().GetPtr();
				TData* pBasePhase = pBasePeakArray->GetPhaseBuffer().GetPtr();
				TData* pBaseMag = pBasePeakArray->GetMagBuffer().GetPtr();
				int* pBaseIndex = pBasePeakArray->GetIndexArray().GetPtr();
				
				// Target peak array iterators
				TData* pTargetFreq = pTargetPeakArray.GetFreqBuffer().GetPtr();
				TData* pTargetMag = pTargetPeakArray.GetMagBuffer().GetPtr();
				TData* pTargetMag2 = pTargetPeakArray2.GetMagBuffer().GetPtr();

				int* pTargetIndex = pTargetPeakArray.GetIndexArray().GetPtr();
				int i; // For the 'for' loops

				for( i=0;i<nPeaks1;i++)
					{
						// Inv: { 0 < i < nPeaks1 }
						// Cota: {nPeaks1}
						// Inv && (nPeaks1 < nPeaks2) = 0 < i < nPeaks1 < nPeaks2
						// lin freq interpolation
						base = *pTargetFreq++;
						*pkFreqBuffer++ = DoLinInterpolation(*pBaseFreq++,base,mFreqInterpolFactor);

						// lin phase interpolation
						*pkPhaseBuffer++ = DoLinInterpolation(*pBasePhase++,base,mFreqInterpolFactor);
						
						// lin mag interpolation
						base = *pBaseMag++; 	
						target = *pTargetMag++;
						tempValue = (base+mMagInterpolFactor*(target-base));
						

						if (i<nPeaks3)
							{
								tempValue = tempValue + mMagInterpolFactor2 * ((*pTargetMag2++)-tempValue);
							}
						else
							{
								tempValue = 0; // FADE OUT TRACK...
								pTargetMag2++;
							}

						*pkMagBuffer++ = tempValue*mMagGain;
						*pkBinPosBuffer++ = -1;
						*pkBinWidthBuffer++ = -1;
						
						// use base array indizes for sinusoidal track indexing ....
						//    		pSynthFrame->SetPeakArrayIndex(i,pBasePeakArray->GetIndex(i));
						*pkIndexArrayBuffer++ = *pBaseIndex++;
					}
			// create new partials
				
				TData magInterpolFactor3magGain = mMagInterpolFactor*mMagInterpolFactor*mMagInterpolFactor*mMagGain;
				TData fundFreq = pSpecFrameBase.GetFundamental().GetFreq();
				TData freq = fundFreq*nPeaks1;
				TData* pTargetPhase = &(pTargetPeakArray.GetPhaseBuffer().GetPtr()[nPeaks1]);

				for(i= nPeaks1;i<nPeaks2;i++, freq+=fundFreq )
					{ 
						if (freq>=mSpectralRange||freq<50) // 50 is a preliminary value 
							{
								pSynthFrame.SetPeakArraySize(i);
								break; // dont create new peaks out of range ...
							}
	      				
						*pkFreqBuffer++ = freq;
						*pkPhaseBuffer++ = *pTargetPhase++;
						*pkMagBuffer++ = (*pTargetMag++)*magInterpolFactor3magGain;
						*pkBinPosBuffer++ = -1;
						*pkBinWidthBuffer++ = -1;
						*pkIndexArrayBuffer++ = i;
					}
			}
		
		// fund freq of BaseFrame
		base = pSpecFrameBase.GetFundamental().GetFreq();
		CLAM_ASSERT(base>=0, "base < o!!\n");

		target = mTargetFreq;
		pSynthFrame.SetFundFreq(base+mFreqInterpolFactor*(target-base));
	
	}

	void InterpolatingSynthesis::DoPitchMod( CSaltoSynthFrame &pSynthFrame)
	{
		SpectralPeakArray*	pBasePeakArray = pSynthFrame.GetPeakArrayPtr();
		TSize nPeaks  = pBasePeakArray->GetnPeaks();
		SpectralPeak  tempPeak;
		TData         randomFactor;//,randomRange;
		int           i,j;
  
		if ( nPeaks > 0 )
			{
				TData invrandomRange = 1/mRandomRange;
				TData invrandomRange10 = invrandomRange / 10;
				TData* pBaseMag = pBasePeakArray->GetMagBuffer().GetPtr();
				TData* pBaseFreq = pBasePeakArray->GetFreqBuffer().GetPtr();
				
				for ( i = 0, j = mRandomIndex; i < nPeaks; i++, j++ )
					{
						if ( j>=MAX_SINES) j = 0;
						randomFactor = mRandomNumber[j];
						
						bool flag = int(randomFactor*100) & 0x00000001; // Parity test
						
						// MRJ: This conditional is not thread-safe, UseRandomDeviations can be changed
						// from the GUI thread !!!!!!!!!
						if ( mUseRandomDeviations && (i>10) && (flag) && mInLoopSynthesis )
							{
								(*pBaseMag++) *= ( 1 -randomFactor*invrandomRange );
								(*pBaseFreq++) *= ( 1 - randomFactor*invrandomRange10 );
							}
						else
							(*pBaseFreq++) *= mPitchFactor;
					}
			}
		
		pSynthFrame.SetFundFreq(pSynthFrame.GetFundFreq()*mPitchFactor);
		
		mRandomIndex+=51;
		if(mRandomIndex>=MAX_SINES) mRandomIndex -= MAX_SINES; // prevent too much periodicity
  
	}

	bool InterpolatingSynthesis::Do()
	{
		return true;
	}
};
