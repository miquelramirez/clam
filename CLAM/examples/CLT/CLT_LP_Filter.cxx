
#include "ErrProcessingObj.hxx"
#include <stdlib.h>
#include <cmath>
#include "CLT_LP_Filter.hxx"


CLT_LP_Filter::CLT_LP_Filter()
{
	mSampleRate = 44100;
	mTwoPiOverSampleRate = (2* M_PI) / mSampleRate;
	mLastOutput = mLastCutoff = mAmountOfCurrent = mAmountOfLast = 0;
}

CLT_LP_Filter::CLT_LP_Filter(TData SampleRate)
{
	mSampleRate = SampleRate;
	mTwoPiOverSampleRate = (2* M_PI) / mSampleRate;
	mLastOutput = mLastCutoff = mAmountOfCurrent = mAmountOfLast = 0;
}

void CLT_LP_Filter::ConcreteConnect(CLT_Descriptor* descr)
{
	descr->activate = Activate;
}

void CLT_LP_Filter::ConnectPort(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
{
	CLAM_ASSERT(sizeof(TData) == sizeof(float), "TData must be float.");
	
	switch (Port)
	{
	case FL_CUTOFF:
		((CLT_LP_Filter *)Instance)->mCutoff = (TData*)DataLocation;
		break;
	case FL_INPUT:
		((CLT_LP_Filter *)Instance)->sIBuffer.GetBuffer().SetPtr((TData*)DataLocation,2048);
		break;
	case FL_OUTPUT:
		((CLT_LP_Filter *)Instance)->sOBuffer.GetBuffer().SetPtr((TData*)DataLocation,2048);
		break;
	}
}

LADSPA_Handle CLT_LP_Filter::Instantiate
( const struct _LADSPA_Descriptor* Descriptor, 
  unsigned long SampleRate )
{
	return new CLT_LP_Filter((TData)SampleRate);
}

void CLT_LP_Filter::Activate(LADSPA_Handle Instance)
{
	CLT_LP_Filter* filter;
	filter = (CLT_LP_Filter*)Instance;
	filter->mLastOutput = 0;
}

void CLT_LP_Filter::Run(LADSPA_Handle Instance, unsigned long SampleCount)
{
	CLT_LP_Filter* filter;
	filter = (CLT_LP_Filter*) Instance;
	sIBuffer.GetBuffer().SetSize(SampleCount);
	sOBuffer.GetBuffer().SetSize(SampleCount);
	filter->Do(sIBuffer, sOBuffer);
}

bool CLT_LP_Filter::Do(Audio &in, Audio &out)
{
	TData size = out.GetSize();
	TData* endoutp = &(out.GetBuffer()[size]);
	TData* outp = &(out.GetBuffer()[0]);
	TData* inp = &(in.GetBuffer()[0]);
	TData cutoff = *mCutoff;
	TData comp;
	TData amountOfCurrent;
	TData amountOfLast;
	TData lastOutput;
	
	if (cutoff != mLastCutoff)
	{
		mLastCutoff = cutoff;
		
		if (mLastCutoff <= 0)
		{
			mAmountOfCurrent = mAmountOfLast = 0;
		}
		else
		{ 
			if (mLastCutoff > mSampleRate * 0.5)
			{
				mAmountOfCurrent = 1;
				mAmountOfLast = 0;
			}
			else
			{
				comp = 2 - cos(mTwoPiOverSampleRate * mLastCutoff);
				mAmountOfLast = comp - sqrt(comp * comp -1);
				mAmountOfCurrent = 1 - mAmountOfLast;
			}
		}
		
	}
	
	amountOfCurrent = mAmountOfCurrent;
	amountOfLast = mAmountOfLast;
	lastOutput = mLastOutput;
	
	while( outp!=endoutp )
	{
		*(outp++) = lastOutput = 
			(amountOfCurrent * *(inp++) + 
			 amountOfLast * lastOutput);
	}
	mLastOutput = lastOutput;
	return true;
}

void initialise_CLT_LP_filter()
{
	
	LPF_Descriptor = new CLT_Descriptor
		(1101,
		 "CLT_lpf",
		 "CLAM Low Pass Filter",
		 "Xavi Rubio (MTG)",
		 "None",
		 3);
	
	
	LPF_Descriptor->SetProperties(0,0,1);
	
	
	CLT_Port * cutoff = new CLT_Port(1,0,0,1,"Cutoff Frequency (Hz)");
	cutoff->CreateHint(1,1,0,1,1,0);
	cutoff->SetLowerBound(0);
	cutoff->SetUpperBound(0.5);
	
	CLT_Port * input = new CLT_Port(1,0,1,0,"Input");
	CLT_Port * output = new CLT_Port(0,1,1,0,"Output");
	
	
	LPF_Descriptor->ConnectPort(cutoff);
	LPF_Descriptor->ConnectPort(input);
	LPF_Descriptor->ConnectPort(output);
	
	
	CONNECT( LPF_Descriptor , CLT_LP_Filter );
	
	registerNewPluginDescriptor(LPF_Descriptor);
}

void finalise_CLT_LP_filter()
{
	delete LPF_Descriptor;
}
