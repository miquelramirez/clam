#include "PhaseManagement.hxx"
#include "Err.hxx"
#include "SpectralPeakArray.hxx"

using namespace CLAM;

Enum::tEnumValue EPhaseGeneration::sEnumValues[] = {
		{EPhaseGeneration::eAlign,"Align"},
		{EPhaseGeneration::eRandom,"Random"},
		{EPhaseGeneration::eContinuation,"Continuation"},
		{0,NULL}
	};

Enum::tValue EPhaseGeneration::sDefault = EPhaseGeneration::eAlign;

void PhaseManagementConfig::DefaultInit()
{
	AddName();
	AddSamplingRate();
	AddMaxSines();
	AddType();
	UpdateData();
	DefaultValues();
}

void PhaseManagementConfig::DefaultValues()
{
	SetSamplingRate(44100);
	SetMaxSines(100);
	SetType(EPhaseGeneration::eContinuation);
}

PhaseManagement::PhaseManagement()
{
	Configure(PhaseManagementConfig());
	Init();
}

PhaseManagement::PhaseManagement(PhaseManagementConfig& c)
{
	Configure(c);
	Init();
}

bool PhaseManagement::ConcreteConfigure(const ProcessingConfig& c)
{
	mConfig=dynamic_cast<const PhaseManagementConfig&> (c);
	return true;
}

void PhaseManagement::Init()
{
	mRandomPhase.Resize(mConfig.GetMaxSines());
	mRandomPhase.SetSize(mConfig.GetMaxSines());
		
	mFrameTime=0;
	mLastPeriodTime=0.0;
	mNextPeriodTime=0.0;
	mLastFundFreq=0.0;

	mLastPeakArray.AddPhaseBuffer();
	mLastPeakArray.AddBinWidthBuffer();
	mLastPeakArray.AddBinPosBuffer();
	mLastPeakArray.AddIndexArray();
	mLastPeakArray.AddIsIndexUpToDate();

	mLastPeakArray.UpdateData();
	mLastPeakArray.SetScale(EScale::eLog);
	// init
	GenerateRandomPhases(mRandomPhase);
	GenerateRandomPhases(mLastPeakArray.GetPhaseBuffer());

	mLastPeakArray.SetIsIndexUpToDate(true);

}


PhaseManagement::~PhaseManagement()
{

}


bool PhaseManagement::Do(Frame& currentFrame)
{
	switch(mConfig.GetType())
	{
		case (EPhaseGeneration::eAlign):
		{
			DoPhaseAlignment(currentFrame.GetSpectralPeakArray(),currentFrame.GetCenterTime(),currentFrame.GetFundamental().GetFreq(0));
			break;
		}
		case (EPhaseGeneration::eContinuation):
		{
			DoPhaseContinuation(currentFrame.GetSpectralPeakArray(),currentFrame.GetCenterTime());
			break;
		}
		case (EPhaseGeneration::eRandom):
		{
			DoRandomPhases(currentFrame.GetSpectralPeakArray());
			break;
		}
	}
	return true;
}

//----------------------------------------------------------------------------//
void
PhaseManagement::ResetPhaseAlignment()
{
	// reset values before starting new thesis thread
	mLastPeriodTime = 0.0;
	mNextPeriodTime = 0.0;
	mLastFundFreq = 0.0;
}
//----------------------------------------------------------------------------//

void
PhaseManagement::DoPhaseAlignment(SpectralPeakArray& peakArray, double currentTime, double fundFreq)
{

	TIndex numPeaks	= peakArray.GetnPeaks();
	double					phase,freq;
  
	//phaseAlignment 
	if (fundFreq>0)	// use phase align only when fundfreq is existing 
	{
		double newPeriodDuration = 1/fundFreq;
		
		if ((mLastPeriodTime!=0.0) && (mLastPeriodTime<currentTime))
		{
			double lastPeriodDuration = mNextPeriodTime - mLastPeriodTime;
			mNextPeriodTime = mLastPeriodTime + newPeriodDuration;
			double averagePeriodDuration = 0.5*lastPeriodDuration + 0.5*newPeriodDuration;

			double timeDiff = currentTime-mLastPeriodTime;

			TIndex nPeriodsElapsed = (TIndex)floor(timeDiff/averagePeriodDuration);    		
		
			if (timeDiff-nPeriodsElapsed*averagePeriodDuration > newPeriodDuration-mFrameTime*0.5)
				nPeriodsElapsed++;
			
			double timePeriodDiff = timeDiff - nPeriodsElapsed*averagePeriodDuration;
			
			double thPeriodPosition = (timeDiff-nPeriodsElapsed*averagePeriodDuration)/newPeriodDuration;
			
			timePeriodDiff = thPeriodPosition*newPeriodDuration;
			
			for (int i = 0;i<numPeaks;i++)
			{
				double phase1 = peakArray.GetPhase(i);
				freq	= peakArray.GetFreq(i);
				phase = phase1+TWO_PI*freq*timePeriodDiff;			
				// in orig. sms we distinguish here perfect harmonic/not perfect harmonic
				phase = phase - floor(phase/TWO_PI)*TWO_PI;	// phase wrapping
				peakArray.SetPhase(i,TData(phase));
			}
			if (nPeriodsElapsed > 0)
			{
				mLastPeriodTime += nPeriodsElapsed*averagePeriodDuration;
				mNextPeriodTime = mLastPeriodTime + newPeriodDuration;
			}
		}
		else
		{
			mLastPeriodTime = currentTime;
			mNextPeriodTime = currentTime + newPeriodDuration;
		}
	}			 
 	else
	{
		mLastPeriodTime	= 0.0;
		mNextPeriodTime	= 0.0;	
	}
	mLastPeakArray.SetIsIndexUpToDate(true);

}
  
//----------------------------------------------------------------------------//

void PhaseManagement::DoRandomPhases(SpectralPeakArray& peakArray)
{
	//MTG::SpectralPeakArray* peakArray = thFrame.GetPeakArrayPtr();
	TIndex numPeaks = peakArray.GetnPeaks();
	
	int							i;
	double					phase,freq;
		
	DataArray& lastPhase=mLastPeakArray.GetPhaseBuffer();
	DataArray& lastFreq=mLastPeakArray.GetFreqBuffer();
	int nPeaks=peakArray.GetnPeaks();
	
	lastPhase.Resize(nPeaks);
	lastPhase.SetSize(nPeaks);
	lastFreq.Resize(nPeaks);
	lastFreq.SetSize(nPeaks);

	for	(i=0;i<numPeaks;i++)
	{
		//phase = peakArray.GetPhase(i);
		freq  = peakArray.GetFreq(i);
		
		//find peak corresponding to current track
		TIndex prevPos =peakArray.GetIndex(i);
		
		// use a set of random phases and calculate each time the correct phase..
  		if (prevPos == -1||prevPos>mLastPeakArray.GetnPeaks()) // new track...
  		{
  			phase = mRandomPhase[i];
  			peakArray.SetPhase(i,TData(phase));

			lastPhase[i]=phase;
			lastFreq[i]=freq;
			/*SpectralPeak tmpPeak;
			mLastPeakArray.InitPeak(tmpPeak);
  			tmpPeak.SetPhase(TData(phase));
			tmpPeak.SetFreq(TData(phase));
			mLastPeakArray.AddSpectralPeak(tmpPeak);*/
  		}
  		else	// track is existing, calculate according phase..
  		{
  			SpectralPeak lastPeak=mLastPeakArray.GetSpectralPeak(prevPos);
			phase = lastPeak.GetPhase() + TWO_PI*((lastPeak.GetFreq()+freq)*0.5*mFrameTime);
  			
  			//phase = phase - floor(phase/TWO_PI)*TWO_PI;
  			while (phase >= TWO_PI) phase = phase -TWO_PI;		// other way..
  			peakArray.SetPhase(i,TData(phase));
  			lastPhase[prevPos]=TData(phase);
			lastFreq[prevPos]=TData(phase);
  		}	
	}
	mLastPeakArray.SetIsIndexUpToDate(true);

   
}


void PhaseManagement::DoPhaseContinuation(SpectralPeakArray& p,TData t)
{
	int i;
	for(i=0;i<p.GetnPeaks();i++)
	{
		TIndex currentIndex=p.GetIndex(i);
		TIndex lastPos=mLastPeakArray.GetPositionFromIndex(currentIndex);
		if(lastPos!=-1)
		{
			SpectralPeak tmpPeak=mLastPeakArray.GetSpectralPeak(lastPos);
			p.SetPhase(i,tmpPeak.GetPhase()+TData(TWO_PI)*TData(0.5)*(tmpPeak.GetFreq()+p.GetFreq(i))*(t-mFrameTime));
//			mLastPeakArray.SetSpectralPeak(lastPos,p.GetSpectralPeak(i));
			
		}
		else
		{
			p.SetPhase(i,/*testing mpLastPhase[i]+*/TData(TWO_PI)*p.GetFreq(i)*(t-mFrameTime));
//			mLastPeakArray.AddSpectralPeak(p.GetSpectralPeak(i));
		}
		p.SetPhase(i,p.GetPhase(i)-floor((TData)(p.GetPhase(i)/TData(TWO_PI)))*TData(TWO_PI));
		
	}
	mFrameTime=t;
	mLastPeakArray=p;
	mLastPeakArray.SetIsIndexUpToDate(true);
}
   
//----------------------------------------------------------------------------//
void
PhaseManagement::GenerateRandomPhases(DataArray& a)
{
	for (int i = 0; i<a.Size();i++)
	{
		a[i] = rand()/TData(RAND_MAX*TWO_PI);
	}
}
//----------------------------------------------------------------------------//
void
PhaseManagement::SetLastPhasesAndFreqs(SpectralPeakArray& peakArray)
{
	DataArray& lastPhase=mLastPeakArray.GetPhaseBuffer();
	DataArray& lastFreq=mLastPeakArray.GetFreqBuffer();
	DataArray& currentPhase= peakArray.GetPhaseBuffer();
	DataArray& currentFreq= peakArray.GetFreqBuffer();
	int nPeaks=peakArray.GetnPeaks();

	lastPhase.Resize(nPeaks);
	lastPhase.SetSize(nPeaks);
	lastFreq.Resize(nPeaks);
	lastFreq.SetSize(nPeaks);

	for (int i=0;i<nPeaks;i++)
	{
		lastPhase[i] = currentPhase[i];
		lastFreq[i]  = currentFreq[i];
	}
	mLastPeakArray.SetIsIndexUpToDate(true);

}
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
