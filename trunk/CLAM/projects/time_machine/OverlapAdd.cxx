#include "OverlapAdd.hxx"


using namespace MTG;


//////////////////////////////////////////////////////////////////////
//
// OverlapAddConfig
//
//////////////////////////////////////////////////////////////////////

/* the configuration object has at least to have a name */

void OverlapAddConfig::Init()
{
  /* the dynamic type takes care if we add an existing attr .. */
  AddName();
	AddHopSize();
	AddFrameSize();
	AddBufferSize();

  /* all attributes are added */
  UpdateData();

  /* set default values */
	SetName("OverlapAdd");
	SetHopSize(0);
	SetFrameSize(0);
	SetBufferSize(0);
}

//////////////////////////////////////////////////////////////////////
//
// OverlapAdd
//
//////////////////////////////////////////////////////////////////////

/* processing object method implementations */

OverlapAdd::OverlapAdd(void)
{
	Configure(OverlapAddConfig());
}

OverlapAdd::OverlapAdd(const OverlapAddConfig &c)
{
	Configure(c);
}

OverlapAdd::~OverlapAdd()
{
}


/* configure the processing object according to the config object */

bool OverlapAdd::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const OverlapAddConfig&>(c);
	mHopSize= mConfig.GetHopSize();
	return true;
}

/* other functions */



/* setting prototypes for faster processing */

bool OverlapAdd::SetPrototypes(){
  return false;}

bool OverlapAdd::UnsetPrototypes(){
  return false;}

/* the supervised Do() function */

bool OverlapAdd::Do(void)
{
	throw(ErrProcessingObj("OverlapAdd::Do(): Supervised mode not implemented"),this);
	return false;
}

bool OverlapAdd::Do(DataArray &in, AudioCircularBuffer<TData> &overlapAddBuffer, DataArray &out)
{
	TInt32 inSize = in.Size();
	TInt32 outSize = out.Size();
	TData *pin = in.GetPtr();
#ifdef PARANOID
	if((inSize >> 1) == mHopSize)
	{
#endif
		overlapAddBuffer.AddToBuffer(mHopSize, pin, mHopSize);
		overlapAddBuffer.OverwriteBuffer(pin+mHopSize, mHopSize);

		//modify write pointer
		overlapAddBuffer.DecreaseWriteIndex(mHopSize);

		//read frame from buffer
		overlapAddBuffer.ReadFromBuffer(out);
#ifdef PARANOID
	}
	else throw Err("OverlapAdd::Do: buffer size and hop size are not adjusted correctly");
#endif

	return true;
} 

bool OverlapAdd::Do(Audio &in, AudioCircularBuffer<TData> &overlapAddBuffer, Audio &out)
{
	return Do(in.GetFloatBuffer(), overlapAddBuffer, out.GetFloatBuffer());
}
