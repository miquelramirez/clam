
#include "ErrProcessingObj.hxx"
#include <stdlib.h>
#include <cmath>
#include "CLT_HP_Filter.hxx"


CLT_HP_Filter::CLT_HP_Filter()
{
		mSampleRate = 44100;
		mTwoPiOverSampleRate = (2* M_PI) / mSampleRate;
		mLastOutput = mLastCutoff = mAmountOfCurrent = mAmountOfLast = 0;
}

CLT_HP_Filter::CLT_HP_Filter(TData SampleRate)
{
		mSampleRate = SampleRate;
		mTwoPiOverSampleRate = (2* M_PI) / mSampleRate;
		mLastOutput = mLastCutoff = mAmountOfCurrent = mAmountOfLast = 0;
}

void CLT_HP_Filter::ConcreteConnect(CLT_Descriptor* descr)
{
		descr->activate = Activate;
}

void CLT_HP_Filter::ConnectPort(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
{
		CLAM_ASSERT(sizeof(TData) == sizeof(float), "TData must be float.");

		switch (Port)
		{
		case FL_CUTOFF:
				((CLT_HP_Filter *)Instance)->mCutoff = (TData*)DataLocation;
				break;
		case FL_INPUT:
				((CLT_HP_Filter *)Instance)->sIBuffer.GetBuffer().SetPtr((TData*)DataLocation,0);
				break;
		case FL_OUTPUT:
				((CLT_HP_Filter *)Instance)->sOBuffer.GetBuffer().SetPtr((TData*)DataLocation,0);
				break;
		}
}

LADSPA_Handle CLT_HP_Filter::Instantiate
( const struct _LADSPA_Descriptor* Descriptor, 
  unsigned long SampleRate )
{
		return new CLT_HP_Filter((TData)SampleRate);
}

////////////falta constructor i activate //////////////
void CLT_HP_Filter::Activate(LADSPA_Handle Instance)
{
		CLT_HP_Filter* filter;
		filter = (CLT_HP_Filter*)Instance;
		filter->mLastOutput = 0;
}

void CLT_HP_Filter::Run(LADSPA_Handle Instance, unsigned long SampleCount)
{
		CLT_HP_Filter* filter;
		filter = (CLT_HP_Filter*) Instance;
		sIBuffer.GetBuffer().SetSize(SampleCount);
		sOBuffer.GetBuffer().SetSize(SampleCount);
		filter->Do(sIBuffer, sOBuffer);
}

bool CLT_HP_Filter::Do(Audio &in, Audio &out)
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
//reject everything
						mAmountOfCurrent = 1;
						mAmountOfLast = 0;
				}
				else
				{ 
						if (mLastCutoff > mSampleRate * 0.5)
						{
								mAmountOfCurrent = mAmountOfLast = 0;
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
				lastOutput = 
						(amountOfCurrent * *(inp) + 
						 amountOfLast * lastOutput);
				
				*(outp++) = *(inp++) - lastOutput;
		}

		mLastOutput = lastOutput;
		return true;
}

void initialise_CLT_HP_filter()
{
		HPF_Descriptor = new CLT_Descriptor
				(1102,
				 "CLT_hpf",
				 "CLAM High Pass Filter",
				 "Xavi Rubio (MTG)",
				 "None",
				 3);

		HPF_Descriptor->SetProperties(0,0,1);

		CLT_Port * cutoff = new CLT_Port(1,0,0,1,"Cutoff Frequency (Hz)");
		cutoff->CreateHint(1,1,0,1,1,0);
		cutoff->SetLowerBound(0);
		cutoff->SetUpperBound(0.5);

		CLT_Port * input = new CLT_Port(1,0,1,0,"Input");
		CLT_Port * output = new CLT_Port(0,1,1,0,"Output");

		HPF_Descriptor->ConnectPort(cutoff);
		HPF_Descriptor->ConnectPort(input);
		HPF_Descriptor->ConnectPort(output);

		CONNECT( HPF_Descriptor , CLT_HP_Filter );


		registerNewPluginDescriptor(HPF_Descriptor);

}

void finalise_CLT_HP_filter()
{
		delete HPF_Descriptor;
}
