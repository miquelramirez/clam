#include "TDWindowing.hxx"
#include "ZeroPhaseWindowing.hxx"


using namespace MTG;


//////////////////////////////////////////////////////////////////////
//
// TDWindowingConfig
//
//////////////////////////////////////////////////////////////////////

/* the configuration object has at least to have a name */

void TDWindowingConfig::Init()
{
  /* the dynamic type takes care if we add an existing attr .. */
  AddName();
	AddWindowingMethod();
	AddOverlapSize();

  /* all attributes are added */
  UpdateData();

  /* set default values */
	SetName("TDWindowing");
	SetWindowingMethod(MTG::EWindowingMethod::ANALYSIS);
	SetOverlapSize(0);
}

//////////////////////////////////////////////////////////////////////
//
// TDWindowing
//
//////////////////////////////////////////////////////////////////////

/* processing object method implementations */
TDWindowing::TDWindowing(void)
{
	Configure(TDWindowingConfig());
}

TDWindowing::TDWindowing(const TDWindowingConfig &c)
{
	Configure(c);
}

TDWindowing::~TDWindowing()
{
}


/* configure the processing object according to the config object */

bool TDWindowing::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const TDWindowingConfig&>(c);
	return true;
}

/* other functions */



/* setting prototypes for faster processing */

bool TDWindowing::SetPrototypes(){
  return false;}

bool TDWindowing::UnsetPrototypes(){
  return false;}

/* the supervised Do() function */

bool TDWindowing::Do(void)
{
	throw(ErrProcessingObj("TDWindowing::Do(): Supervised mode not implemented"),this);
	return false;
}

/* the  unsupervised Do() function */

bool TDWindowing::Do(TData *pin, TInt32 inSize, TData *pout, TInt32 outSize, TData *pwin, TInt32 winSize)
{
	switch(mConfig.GetWindowingMethod())
	{
		case 0:      //analysis				if(winSize % 2 != 0) //do if array length is even
			{
			#ifdef PARANOID
				if(inSize != winSize )
					throw Err("ZeroPhaseWindowing::Do(): window and input buffer are not of the same size -> in this version be sure that inSize = winSize -1");
				if(inSize != outSize )
					throw Err("ZeroPhaseWindowing::Do(): in and out buffer have not the same size");
			#endif
				TInt32 inMiddle = inSize >> 1;
				for(int i=0, k=inMiddle; i<inMiddle; i++, k++)
				{
					pout[i] = pin[i] * pwin[i];
					pout[k] = pin[k] * pwin[k];
				}
			}
			break;
		case 1:     //synthesis windowing
			{
				TInt32 overlapSize = mConfig.GetOverlapSize();
				//check if window and in and out buffer are of the same size 
			#ifdef PARANOID
				if((overlapSize << 1) != outSize) // (2*overlapSize != outSize)
					throw Err("ZeroPhaseWindowing::Do(): overlapSize and output buffer are not of the same size");
			#endif
				TInt32 winMiddle = (winSize-1) >> 1; // (winSize-1)/2
				TInt32 inMiddle = inSize >> 1; // inSize/2
				for(int i=0, k=inMiddle-overlapSize ; i<overlapSize; k++, i++)	//for zero-phase filtering
				{
					pout[i]             = pin[k] * pwin[winMiddle-overlapSize+i];
					pout[overlapSize+i] = pin[inMiddle+i] * pwin[winMiddle+i];
				}
			}
			break;
	}
	return true;
}


/* overloaded unsupervised Do() function */

bool TDWindowing::Do(Audio &in, Audio &out, DataArray &window)
{
	return Do(in.GetFloatBuffer(), out.GetFloatBuffer(), window);
}

bool TDWindowing::Do(Audio &in, Audio &out, Window &window)
{
	return Do(in.GetFloatBuffer(), out.GetFloatBuffer(), window.GetWindowBuffer());
}

bool TDWindowing::Do(DataArray &in, DataArray &out, Window &window)
{
	return Do(in, out, window.GetWindowBuffer());
}

bool TDWindowing::Do(DataArray &in, DataArray &out, DataArray &window)
{
	//return Do(in.GetPtr(), in.AllocatedSize(), out.GetPtr(), out.AllocatedSize(), window.GetPtr(), window.AllocatedSize());
	return Do(in.GetPtr(), in.Size(), out.GetPtr(), out.Size(), window.GetPtr(), window.Size());
}



