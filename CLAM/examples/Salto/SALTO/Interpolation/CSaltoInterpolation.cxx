#include <pthread.h>
#include "CSaltoInterpolation.hxx"
#include "SpectralPeak.hxx"
#include "SpectralPeakArray.hxx"
#include "Assert.hxx"
#include "HeapDbg.hxx"
#include <iostream>

//pthread_cond_t condPA = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t mutPA = PTHREAD_MUTEX_INITIALIZER;

namespace CLAM
{

	void CSaltoInterpolationConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

//CSaltoInterpolation::CSaltoInterpolation(CSaltoParameter* pParams)
//	CSaltoInterpolation::CSaltoInterpolation(Parameters* pParams)
	CSaltoInterpolation::CSaltoInterpolation( const CSaltoInterpolationConfig& cfg ):
	mOneTrackAlreadyStarted(false),
	mRandomIndex(0)
	{
		Configure( cfg );
	}

	CSaltoInterpolation::CSaltoInterpolation(  ):
	mOneTrackAlreadyStarted(false),
	mRandomIndex(0)
	{
		CSaltoInterpolationConfig cfg;

		Configure( cfg );
	}


	CSaltoInterpolation::~CSaltoInterpolation()
	{
	}

	bool CSaltoInterpolation::ConcreteConfigure( const ProcessingConfig& cfg) throw( std::bad_cast )
	{
		mConfig = dynamic_cast< const CSaltoInterpolationConfig& >( cfg );

		mSpectralRange = mConfig.GetSpectralRange();
		mpParams = mConfig.GetParams();

		for (int i = 0; i<MAX_SINES;i++)
		{
			mRandomNumber[i] = (TFloat32)rand()/RAND_MAX;
		}

		return true;
	}

	bool CSaltoInterpolation::ConcreteStart()
	{
		return true;
	}

	bool CSaltoInterpolation::ConcreteStop()
	{
		return true;
	}
	//----------------------------------------------------------------------------//
	// Interpolation of two SpectralFrames -> SynthFrame
	//----------------------------------------------------------------------------//
	void CSaltoInterpolation::DoInterpolation(Frame *pSpecFrameBase,	// Spectral Frame Base
											  SpectralPeakArray *pTargetPeakArray,// Target Peak Array
											  double targetFundFreq,      // target Fundamental Frequency
											  double magInterpolFactor,
											  double freqInterpolFactor,  // overall interpolation Factor
											  double magGain,             // overall Magnitude Gain
											  CSaltoSynthFrame *pSynthFrame, // result is store in synth frame
											  bool doFreqIP,              // use linear Frequency interpolation
											  bool doPhaseIP,						  // use linear phase interpolation			
                                        		SpectralPeakArray *pTargetPeakArray2,// second target for timbre interpol only !																			
											  double magInterpolFactor2 ) // interpolation for target 2
	{	
	  SpectralPeakArray*	pBasePeakArray = &pSpecFrameBase->GetSpectralPeakArray(); //BUGFRAME!
	  double base,target,tempValue;
		
		// this should be elaborated ....
		TSize nPeaks1 = pBasePeakArray->GetnPeaks();
		TSize nPeaks2 = pTargetPeakArray->GetnPeaks();
		TSize nPeaks3 = 0;
		if (pTargetPeakArray2!=NULL)
		  nPeaks3 = pTargetPeakArray2->GetnPeaks();
		
	  if ((nPeaks1>=nPeaks2||nPeaks1==0)||(mpParams->GetUseRandomLoop()))//(nPeaks1>=nPeaks2||nPeaks1==0)// for now always true.... future:nPeaks1>=nPeaks2 ||pTargetPeakArray2!=NULL
	  {
    		pSynthFrame->SetPeakArraySize(nPeaks1);
    		for(int i=0;i<nPeaks1;i++)
    		{
    			// lin freq interpolation
    			base = pBasePeakArray->GetFreq(i);
    			if (doFreqIP && i<nPeaks2)
    			{
							pSynthFrame->GetPeakArray().GetFreqBuffer()[i] = DoLinInterpolation(base,pTargetPeakArray->GetFreq(i),freqInterpolFactor);
    			}
    			else
    			{
						pSynthFrame->GetPeakArray().GetFreqBuffer()[i] = base;
    			}
    			
    			// lin phase interpolation
    			base = pBasePeakArray->GetPhase(i);
    			if (doPhaseIP&&i<nPeaks2)
    			{
						pSynthFrame->GetPeakArray().GetPhaseBuffer()[i] = DoLinInterpolation(base,pTargetPeakArray->GetFreq(i),freqInterpolFactor);
    			}
    			else
    			{
						pSynthFrame->GetPeakArray().GetPhaseBuffer()[i] = base;
    			}
    			
    			// lin mag interpolation
    			base = pBasePeakArray->GetMag(i); 	
    			if (i<nPeaks2)
    			{	
      			target = pTargetPeakArray->GetMag(i);
    			}
    			else
    			{
    			  target = 0; // if there is no matching peak , set this track to zero
    			}
    			tempValue = (base+magInterpolFactor*(target-base));
    			
    			if (pTargetPeakArray2!=NULL)  // timbre interpolation and transition interpolation
    		  {
    			  if (i<nPeaks3&&i<nPeaks2&&i<nPeaks1)
    			  {
    				tempValue = tempValue + magInterpolFactor2 * (pTargetPeakArray2->GetMag(i)-tempValue);
    			  }
    			  else
    			  {
    				tempValue = 0; // FADE OUT TRACK...
    			  }
    			}
					pSynthFrame->GetPeakArray().GetMagBuffer()[i] = tempValue*magGain;
					pSynthFrame->GetPeakArray().GetBinPosBuffer()[i] = -1;
					pSynthFrame->GetPeakArray().GetBinWidthBuffer()[i] = -1;

  				// use base array indizes for sinusoidal track indexing ....
	//    		pSynthFrame->SetPeakArrayIndex(i,pBasePeakArray->GetIndex(i));
					pSynthFrame->GetPeakArray().GetIndexArray()[i] = pBasePeakArray->GetIndex(i);
    		}
	  }
	  else
	  {
				int i;
		  pSynthFrame->SetPeakArraySize(nPeaks2);

    		for(i= 0;i<nPeaks1;i++)
    		{
    			// lin freq interpolation
    			base = pBasePeakArray->GetFreq(i);
    			if (doFreqIP&&i<nPeaks2)
    			{
						pSynthFrame->GetPeakArray().GetFreqBuffer()[i] = DoLinInterpolation(base,pTargetPeakArray->GetFreq(i),freqInterpolFactor);
    			}
    			else
    			{
						pSynthFrame->GetPeakArray().GetFreqBuffer()[i] = base;
    			}
    			
    			// lin phase interpolation
    			base = pBasePeakArray->GetPhase(i);
    			if (doPhaseIP&&i<nPeaks2)
    			{
						pSynthFrame->GetPeakArray().GetPhaseBuffer()[i] = DoLinInterpolation(base,pTargetPeakArray->GetFreq(i),freqInterpolFactor);
    			}
    			else
    			{
						pSynthFrame->GetPeakArray().GetPhaseBuffer()[i] = base;
    			}
    			
    			// lin mag interpolation
    			base = pBasePeakArray->GetMag(i);
    			if (i<nPeaks2)
    			{	
      			target	=	pTargetPeakArray->GetMag(i);
    			}
    			else
    			{
    			  target = 0; // if there is no matching peak , set this track to zero
    			}
    			
    			tempValue = (base+magInterpolFactor*(target-base));
    			
    			if (pTargetPeakArray2!=NULL)  // timbre interpolation and transition interpolation
    		  {
    			  if (i<nPeaks3&&i<nPeaks2&&i<nPeaks1)
    			  {
    				tempValue = tempValue + magInterpolFactor2 * (pTargetPeakArray2->GetMag(i)-tempValue);
    			  }
    			  else
    			  {
    				tempValue = 0; // FADE OUT TRACK...
    			  }
    			}
					pSynthFrame->GetPeakArray().GetMagBuffer()[i] = tempValue*magGain;

					pSynthFrame->GetPeakArray().GetBinPosBuffer()[i] = -1;  			

					pSynthFrame->GetPeakArray().GetBinWidthBuffer()[i] = -1;  			

    			// use base array indizes for sinusoidal track indexing ....
	//				pSynthFrame->GetPeakArray().GetIndexArray().AddElem(pBasePeakArray->GetIndex(i)); //deprecated
					pSynthFrame->GetPeakArray().GetIndexArray()[i] = pBasePeakArray->GetIndex(i);

    		}
    		// create new partials
    		for(i= nPeaks1;i<nPeaks2;i++)
    		{ 
    		  double mag    = pTargetPeakArray->GetMag(i)*magInterpolFactor*magInterpolFactor*magInterpolFactor;
    		  double phase  = pTargetPeakArray->GetPhase(i);
    		  double freq   = pSpecFrameBase->GetFundamental().GetFreq()*i;
    		  
    		  if (freq>=mSpectralRange||freq<50) // 50 is a preliminary value 
    		  {
    			pSynthFrame->SetPeakArraySize(i);
    			break; // dont create new peaks out of range ...
    		  }

					pSynthFrame->GetPeakArrayPtr()->GetFreqBuffer()[i] = freq;
					pSynthFrame->GetPeakArrayPtr()->GetPhaseBuffer()[i] = phase;
					pSynthFrame->GetPeakArrayPtr()->GetMagBuffer()[i] = mag*magGain;
					pSynthFrame->GetPeakArrayPtr()->GetBinPosBuffer()[i] = -1;
					pSynthFrame->GetPeakArrayPtr()->GetBinWidthBuffer()[i] = -1;
					pSynthFrame->GetPeakArrayPtr()->GetIndexArray()[i] = i;
    		}
	  }

		// fund freq of BaseFrame
		base = pSpecFrameBase->GetFundamental().GetFreq();
		CLAM_ASSERT(base>=0, "base < o!!\n");
		if (doFreqIP)
		{
		  target = targetFundFreq;
		  pSynthFrame->SetFundFreq(base+freqInterpolFactor*(target-base));
		}
		else
		{
		  pSynthFrame->SetFundFreq(base);
		}


		//This is done to avoid that DisplaypeakArray() in CSaltoEditor tryes to draw a peak array
		//that is not yet filled

	//	pthread_mutex_lock(&mutPA);
	//  pthread_cond_signal(&condPA);
	//  pthread_mutex_unlock(&mutPA);
	}
	
	void CSaltoInterpolation::DoInterpolation2(Frame *pSpecFrameBase,	// Spectral Frame Base
											  SpectralPeakArray *pTargetPeakArray,// Target Peak Array
											  double targetFundFreq,      // target Fundamental Frequency
											  double magInterpolFactor,
											  double freqInterpolFactor,  // overall interpolation Factor
											  double magGain,             // overall Magnitude Gain
											  CSaltoSynthFrame *pSynthFrame, // result is store in synth frame
											  bool doFreqIP,              // use linear Frequency interpolation
											  bool doPhaseIP,						  // use linear phase interpolation			
                                        		SpectralPeakArray *pTargetPeakArray2,// second target for timbre interpol only !																			
											  double magInterpolFactor2 ) // interpolation for target 2
	{	
		SpectralPeakArray*	pBasePeakArray = &pSpecFrameBase->GetSpectralPeakArray(); //BUGFRAME!
		double base,target,tempValue;
		
		// this should be elaborated ....
		TSize nPeaks1 = pBasePeakArray->GetnPeaks();
		TSize nPeaks2 = pTargetPeakArray->GetnPeaks();
		TSize nPeaks3 = 0;
		if (pTargetPeakArray2!=NULL)
			nPeaks3 = pTargetPeakArray2->GetnPeaks();
		
		if ((nPeaks1>=nPeaks2||nPeaks1==0)||(mpParams->GetUseRandomLoop()))//(nPeaks1>=nPeaks2||nPeaks1==0)// for now always true.... future:nPeaks1>=nPeaks2 ||pTargetPeakArray2!=NULL
			{
				// Pre: { nPeaks1>=nPeaks2 v nPeaks1==0 v mpParams->GetUseRandomLoop()}
				pSynthFrame->SetPeakArraySize(nPeaks1);
				// Interpolated peak array iterators

				if ( nPeaks1 == 0 ) return;
				
				TData* pkFreqBuffer = pSynthFrame->GetPeakArray().GetFreqBuffer().GetPtr();;
				TData* pkMagBuffer = pSynthFrame->GetPeakArray().GetMagBuffer().GetPtr();
				TData* pkPhaseBuffer = pSynthFrame->GetPeakArray().GetPhaseBuffer().GetPtr();
				TData* pkBinPosBuffer = pSynthFrame->GetPeakArray().GetBinPosBuffer().GetPtr();
				TData* pkBinWidthBuffer = pSynthFrame->GetPeakArray().GetBinWidthBuffer().GetPtr();
				int* pkIndexArrayBuffer = pSynthFrame->GetPeakArray().GetIndexArray().GetPtr();
	
		
				// Base peak array iterators
		//		pBasePeakArray->Debug();
				TData* pBaseFreq = pBasePeakArray->GetFreqBuffer().GetPtr();
				TData* pBasePhase = pBasePeakArray->GetPhaseBuffer().GetPtr();
				TData* pBaseMag = pBasePeakArray->GetMagBuffer().GetPtr();
				int* pBaseIndex = pBasePeakArray->GetIndexArray().GetPtr();
				
				// Target peak array iterators
				TData* pTargetFreq = pTargetPeakArray->GetFreqBuffer().GetPtr();
				TData* pTargetMag = pTargetPeakArray->GetMagBuffer().GetPtr();
				TData* pTargetMag2 = NULL;
				if ( pTargetPeakArray2!=NULL)
					pTargetMag2 = pTargetPeakArray2->GetMagBuffer().GetPtr();

				int* pTargetIndex = pTargetPeakArray->GetIndexArray().GetPtr();

				for(int i=0;i<nPeaks1;i++)
					{
						// lin freq interpolation
						base = *pTargetFreq++;
						if (doFreqIP && i<nPeaks2)
							{
								*pkFreqBuffer++ = DoLinInterpolation(*pBaseFreq++,base,freqInterpolFactor);
							}
						else
							{
								*pkFreqBuffer++ = *pBaseFreq++;
							}
						
						// lin phase interpolation
						if (doPhaseIP&&i<nPeaks2)
							{
								*pkPhaseBuffer++ = DoLinInterpolation(*pBasePhase++,base,freqInterpolFactor);
							}
						else
							{
								*pkPhaseBuffer++ = *pBasePhase++;
							}
						
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
						tempValue = (base+magInterpolFactor*(target-base));
						
						if (pTargetPeakArray2!=NULL)  // timbre interpolation and transition interpolation
							{
								if (i<nPeaks3&&i<nPeaks2)
									{
										tempValue = tempValue + magInterpolFactor2 * ((*pTargetMag2++)-tempValue);
									}
								else
									{
										tempValue = 0; // FADE OUT TRACK...
										pTargetMag2++;
									}
							}
						*pkMagBuffer++ = tempValue*magGain;
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

				pSynthFrame->SetPeakArraySize(nPeaks2);
				
				// Interpolated peak array iterators
				TData* pkMagBuffer = pSynthFrame->GetPeakArray().GetMagBuffer().GetPtr();
				TData* pkFreqBuffer = pSynthFrame->GetPeakArray().GetFreqBuffer().GetPtr();
				TData* pkPhaseBuffer = pSynthFrame->GetPeakArray().GetPhaseBuffer().GetPtr();
				TData* pkBinPosBuffer = pSynthFrame->GetPeakArray().GetBinPosBuffer().GetPtr();
				TData* pkBinWidthBuffer = pSynthFrame->GetPeakArray().GetBinWidthBuffer().GetPtr();
				int* pkIndexArrayBuffer = pSynthFrame->GetPeakArray().GetIndexArray().GetPtr();
				
				// Base peak array iterators
				TData* pBaseFreq = pBasePeakArray->GetFreqBuffer().GetPtr();
				TData* pBasePhase = pBasePeakArray->GetPhaseBuffer().GetPtr();
				TData* pBaseMag = pBasePeakArray->GetMagBuffer().GetPtr();
				int* pBaseIndex = pBasePeakArray->GetIndexArray().GetPtr();
				
				// Target peak array iterators
				TData* pTargetFreq = pTargetPeakArray->GetFreqBuffer().GetPtr();
				TData* pTargetMag = pTargetPeakArray->GetMagBuffer().GetPtr();
				TData* pTargetMag2 = NULL;
				if ( pTargetPeakArray2!=NULL)
					pTargetMag2 = pTargetPeakArray2->GetMagBuffer().GetPtr();

				int* pTargetIndex = pTargetPeakArray->GetIndexArray().GetPtr();
				int i; // For the 'for' loops

				for( i=0;i<nPeaks1;i++)
					{
						// Inv: { 0 < i < nPeaks1 }
						// Cota: {nPeaks1}
						// Inv && (nPeaks1 < nPeaks2) = 0 < i < nPeaks1 < nPeaks2
						// lin freq interpolation
						base = *pTargetFreq++;
						if (doFreqIP )
							{
								*pkFreqBuffer++ = DoLinInterpolation(*pBaseFreq++,base,freqInterpolFactor);
							}
						else
							{
								*pkFreqBuffer++ = *pBaseFreq++;
							}
						
						// lin phase interpolation
						if (doPhaseIP )
							{
								*pkPhaseBuffer++ = DoLinInterpolation(*pBasePhase++,base,freqInterpolFactor);
							}
						else
							{
								*pkPhaseBuffer++ = *pBasePhase++;
							}
						
						// lin mag interpolation
						base = *pBaseMag++; 	
						target = *pTargetMag++;
						tempValue = (base+magInterpolFactor*(target-base));
						
						if (pTargetPeakArray2!=NULL)  // timbre interpolation and transition interpolation
							{
								if (i<nPeaks3)
									{
										tempValue = tempValue + magInterpolFactor2 * ((*pTargetMag2++)-tempValue);
									}
								else
									{
										tempValue = 0; // FADE OUT TRACK...
										pTargetMag2++;
									}
							}
						*pkMagBuffer++ = tempValue*magGain;
						*pkBinPosBuffer++ = -1;
						*pkBinWidthBuffer++ = -1;
						
						// use base array indizes for sinusoidal track indexing ....
						//    		pSynthFrame->SetPeakArrayIndex(i,pBasePeakArray->GetIndex(i));
						*pkIndexArrayBuffer++ = *pBaseIndex++;
					}
			// create new partials
				
				TData magInterpolFactor3magGain = magInterpolFactor*magInterpolFactor*magInterpolFactor*magGain;
				TData fundFreq = pSpecFrameBase->GetFundamental().GetFreq();
				TData freq = fundFreq*nPeaks1;
				TData* pTargetPhase = &(pTargetPeakArray->GetPhaseBuffer().GetPtr()[nPeaks1]);

				for(i= nPeaks1;i<nPeaks2;i++, freq+=fundFreq )
					{ 
						if (freq>=mSpectralRange||freq<50) // 50 is a preliminary value 
							{
								pSynthFrame->SetPeakArraySize(i);
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
		base = pSpecFrameBase->GetFundamental().GetFreq();
		CLAM_ASSERT(base>=0, "base < o!!\n");
		if (doFreqIP)
			{
				target = targetFundFreq;
				pSynthFrame->SetFundFreq(base+freqInterpolFactor*(target-base));
		}
		else
			{
				pSynthFrame->SetFundFreq(base);
			}
	
		CLAM_CHECK_HEAP("DoInterpolation2 está corrompiendo el heap!");
	}
	

	
	//----------------------------------------------------------------------------//
	void CSaltoInterpolation::DoPitchMod(CSaltoSynthFrame *pSynthFrame,double pitchFactor)
	{
	  SpectralPeakArray*	pBasePeakArray = pSynthFrame->GetPeakArrayPtr();
		TSize nPeaks  = pBasePeakArray->GetnPeaks();
		SpectralPeak			  tempPeak;
		TData           randomFactor,randomRange;
	  int             i,j;
  
	  randomRange = mpParams->GetRandomRange();

		for (i=0,j=mRandomIndex;i<nPeaks;i++,j++)
		{
		  if (j>=MAX_SINES) j=0;
		  randomFactor = mRandomNumber[j];
	//	  tempPeak = pBasePeakArray->GetSpectralPeak(i);
		  
		  bool flag = (int(mRandomNumber[j]*100)%2); // affect only some partials
		  
		  if(mpParams->GetUseRandomDeviations()&&(i>10)&&(flag==true)&&mpParams->GetInLoopSynthesis())
		  {
	//	    tempPeak.SetMag(tempPeak.GetMag()*(1-randomFactor/randomRange));
			pBasePeakArray->GetMagBuffer()[i] *= (1-randomFactor/randomRange);
	//	    tempPeak.SetFreq(tempPeak.GetFreq()*pitchFactor*(1-randomFactor/randomRange/10));
			pBasePeakArray->GetFreqBuffer()[i] *= (1-randomFactor/randomRange/10);
		  }
		  else
		  {
	//	    tempPeak.SetFreq(tempPeak.GetFreq()*pitchFactor);   // OPTIMIZE !!
			pBasePeakArray->GetFreqBuffer()[i] *= pitchFactor;
		  }
	//	  pBasePeakArray->SetSpectralPeak(i,tempPeak);
	  }
	  pSynthFrame->SetFundFreq(pSynthFrame->GetFundFreq()*pitchFactor);
  
	  mRandomIndex+=51;
	  if(mRandomIndex>=MAX_SINES) mRandomIndex -= MAX_SINES; // prevent too much periodicity
  
	}

	void CSaltoInterpolation::DoPitchMod2( CSaltoSynthFrame* pSynthFrame, double pitchFactor )
	{
		SpectralPeakArray*	pBasePeakArray = pSynthFrame->GetPeakArrayPtr();
		TSize nPeaks  = pBasePeakArray->GetnPeaks();
		SpectralPeak  tempPeak;
		TData         randomFactor,randomRange;
		int           i,j;
  
		if ( nPeaks > 0 )
			{
				randomRange = mpParams->GetRandomRange();
				TData invrandomRange = 1/randomRange;
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
						if ( mpParams->GetUseRandomDeviations()&&(i>10)&&(flag)&&mpParams->GetInLoopSynthesis())
							{
								(*pBaseMag++) *= ( 1 -randomFactor*invrandomRange );
								(*pBaseFreq++) *= ( 1 - randomFactor*invrandomRange10 );
							}
						else
							(*pBaseFreq++) *= pitchFactor;
					}
			}
		
		pSynthFrame->SetFundFreq(pSynthFrame->GetFundFreq()*pitchFactor);
		
		mRandomIndex+=51;
		if(mRandomIndex>=MAX_SINES) mRandomIndex -= MAX_SINES; // prevent too much periodicity
	}

	//----------------------------------------------------------------------------//
	inline double
	CSaltoInterpolation::DoLinInterpolation(double base,double target,double ipFactor)
	{
	  return base+ipFactor*(target-base);
	}
	//----------------------------------------------------------------------------//

} // namespace CLAM
