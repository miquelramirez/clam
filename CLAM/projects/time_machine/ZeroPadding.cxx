#include "ZeroPadding.hxx"


using namespace MTG;


//////////////////////////////////////////////////////////////////////
//
// ZeroPaddingConfig
//
//////////////////////////////////////////////////////////////////////

/* the configuration object has at least to have a name */

void ZeroPaddingConfig::Init()
{
  /* the dynamic type takes care if we add an existing attr .. */
	AddName();
  AddInSize();
	AddOutSize();
	AddForcePowerOfTwo();
	AddZeroPhaseUsed();

  /* all attributes are added */
  UpdateData();

  /* set default values */
	SetName("ZeroPadding");
	SetInSize(1);
	SetOutSize(1);
	SetForcePowerOfTwo(false);
	SetZeroPhaseUsed(true);
}

//////////////////////////////////////////////////////////////////////
//
// ZeroPadding
//
//////////////////////////////////////////////////////////////////////

/* processing object method implementations */

ZeroPadding::ZeroPadding(void)
{
	Configure(ZeroPaddingConfig());
}

ZeroPadding::ZeroPadding(const ZeroPaddingConfig &c)
{
	Configure(c);
}

ZeroPadding::~ZeroPadding()
{
}


/* configure the processing object according to the config object */

bool ZeroPadding::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const ZeroPaddingConfig&>(c);
	return true;
}

/* other functions */



/* setting prototypes for faster processing */

bool ZeroPadding::SetPrototypes(){
  return false;}

bool ZeroPadding::UnsetPrototypes(){
  return false;}

/* the supervised Do() function */

bool ZeroPadding::Do(void)
{
	throw(ErrProcessingObj("ZeroPadding::Do(): Supervised mode not implemented"),this);
	return false;
}

/* the  unsupervised Do() function */

bool ZeroPadding::Do(TData *pinputBuffer, TInt32 inSize, TData *poutputBuffer, TInt32 outSize)
{
	TInt32 inputSize = mConfig.GetInSize();
	TInt32 outputSize = mConfig.GetOutSize();
	bool zeroPhaseUsed = mConfig.GetZeroPhaseUsed();

	float zeroPaddingFactor=((float)outputSize/inputSize);
	if (zeroPaddingFactor > 1)
	{
		if(zeroPhaseUsed)
		{
			int middlePoint = inputSize >> 1;
			int nSamplesHalfWindow = middlePoint;
			memset(poutputBuffer, 0, sizeof(TData)*outputSize);
			memcpy(poutputBuffer, pinputBuffer, nSamplesHalfWindow*sizeof(TData));
			memcpy(poutputBuffer+outputSize-middlePoint, pinputBuffer+middlePoint, nSamplesHalfWindow*sizeof(TData));
		}
		else
		{
			int beginPoint= (outputSize-inputSize) >> 1;
			memset(poutputBuffer, 0, sizeof(TData)*outputSize);
			memcpy(poutputBuffer+beginPoint, pinputBuffer, sizeof(TData)*inputSize);
		}
	}
	else if(zeroPaddingFactor < 1)
	{
		if(zeroPhaseUsed)
		{
			int middlePoint = inputSize >> 1;
			int nSamplesHalfWindow = outputSize >> 1;
			memset(poutputBuffer, 0, sizeof(TData)*(outputSize));			
			memcpy(poutputBuffer, pinputBuffer, nSamplesHalfWindow*sizeof(TData));
			memcpy(poutputBuffer+nSamplesHalfWindow, pinputBuffer+inputSize-nSamplesHalfWindow, nSamplesHalfWindow*sizeof(TData));
		}
		else
		{
			int middlePoint = (inputSize-1) >> 1;
			memset(poutputBuffer, 0, sizeof(TData)*(outputSize));
			int overlappedSegment = (inputSize-outputSize) >> 1;
			int leftIndex=overlappedSegment, rightIndex=inputSize-overlappedSegment;
			memcpy(poutputBuffer, pinputBuffer+leftIndex, sizeof(TData)*(rightIndex-leftIndex));
		}
	}
	else 
	{
		//@TODO: check if there alternatives for the next line
		memcpy(poutputBuffer, pinputBuffer, sizeof(TData)*outputSize);
	}

	return true;
}


/* overloaded unsupervised Do() function */

bool ZeroPadding::Do(Audio &in, Audio &out)
{
	bool cfgForcePowerOfTwo = mConfig.GetForcePowerOfTwo();
	if(in.GetSize() != mConfig.GetInSize())
		mConfig.SetInSize(in.GetSize());
	
	if(mConfig.GetInSize() > mConfig.GetOutSize() && mConfig.GetForcePowerOfTwo())
		mConfig.SetForcePowerOfTwo(false);
	
	if(out.GetSize() != mConfig.GetOutSize())
	{
		if(mConfig.GetForcePowerOfTwo())
		{
			mConfig.SetOutSize(CalculatePowerOfTwo(mConfig.GetOutSize()));
			out.SetSize(mConfig.GetOutSize());
		}
		else
			out.SetSize(mConfig.GetOutSize());
	}
	return Do(in.GetFloatBuffer().GetPtr(), in.GetSize(), out.GetFloatBuffer().GetPtr(), out.GetSize());
}

bool ZeroPadding::Do(Audio &in)
{
	AudioConfig cfg;
	cfg.SetSize(mConfig.GetOutSize());
	Audio out(cfg);
	if(Do(in.GetFloatBuffer().GetPtr(), in.GetSize(), out.GetFloatBuffer().GetPtr(), out.GetSize()))
	{
		in = out;
		return true;
	}
	else return false;
}

bool ZeroPadding::Do(Array<TData> &in, Array<TData> &out)
{
	if(in.Size() != mConfig.GetInSize())
		mConfig.SetInSize(in.Size());
	
	if(mConfig.GetInSize() > mConfig.GetOutSize() && mConfig.GetForcePowerOfTwo())
		mConfig.SetForcePowerOfTwo(false);
	
	if(out.Size() != mConfig.GetOutSize())
	{
		if(mConfig.GetForcePowerOfTwo())
		{
			mConfig.SetOutSize(CalculatePowerOfTwo(out.Size()));
			TInt32 outSize = mConfig.GetOutSize();
			out.Resize(outSize);
			out.SetSize(outSize);
		}
		else
		{
			TInt32 outSize = mConfig.GetOutSize();
			out.Resize(outSize);
			out.SetSize(outSize);
		}
	}
	Do(in.GetPtr(), in.Size(), out.GetPtr(), out.Size());
	return true;
}

bool ZeroPadding::Do(Array<TData> &in)
{
	Array<TData> out;
	TInt32 outSize = mConfig.GetOutSize();
	out.Resize(outSize);
	out.SetSize(outSize);

	if(Do(in.GetPtr(), in.Size(), out.GetPtr(), out.Size()))
	{
		in = out;
		return true;
	}
	else return false;
}

TInt32 ZeroPadding::CalculatePowerOfTwo(TInt32 size)
{
	int tmp = size;
	int outputSize = 1;
	while (tmp) 
	{
	 	outputSize=outputSize << 1;
	 	tmp=tmp >> 1;
	}
	if(outputSize == size << 1)
		outputSize = outputSize >> 1;
	return outputSize;
}
