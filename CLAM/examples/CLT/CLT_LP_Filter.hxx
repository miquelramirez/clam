#ifndef _CLT_LP_FILTER
#define _CLT_LP_FILTER


#include "CLT_MonoPlugin.hxx"

#define FL_CUTOFF 0
#define FL_INPUT  1
#define FL_OUTPUT 2

class CLT_LP_Filter : public CLT_MonoPlugin
{
public:

	TData          mSampleRate;
	TData mTwoPiOverSampleRate;
	TData          mLastOutput;
	TData          mLastCutoff;
	TData     mAmountOfCurrent;
	TData        mAmountOfLast;
	TData*             mCutoff;
	
	CLT_LP_Filter();
	CLT_LP_Filter(TData sampleRate);
	virtual ~CLT_LP_Filter(){}
	
	bool Do(void){return true;}
	inline bool Do(Audio &in, Audio &out);
	
	
	static LADSPA_Handle Instantiate
	( const struct _LADSPA_Descriptor* Descriptor, 
	  unsigned long SampleRate );
	static void ConnectPort (LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation);
	static void Run(LADSPA_Handle Instance, unsigned long SampleCount);
	static void Activate(LADSPA_Handle Instance);
	static void ConcreteConnect(CLT_Descriptor* descr);
	
};

static CLT_Descriptor * LPF_Descriptor=NULL;
void initialise_CLT_LP_filter();
void finalise_CLT_LP_filter();
#endif
