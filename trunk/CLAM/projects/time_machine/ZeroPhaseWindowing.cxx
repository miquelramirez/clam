#include "ZeroPhaseWindowing.hxx"


using namespace MTG;


//////////////////////////////////////////////////////////////////////
//
// ZeroPhaseWindowingConfig
//
//////////////////////////////////////////////////////////////////////

/* the configuration object has at least to have a name */

void ZeroPhaseWindowingConfig::Init()
{
  /* the dynamic type takes care if we add an existing attr .. */
  AddName();
	AddWindowingMethod();
	AddOverlapSize();

  /* all attributes are added */
  UpdateData();

  /* set default values */
	SetWindowingMethod(MTG::EWindowingMethod::ANALYSIS);
	SetOverlapSize(0);
}

//////////////////////////////////////////////////////////////////////
//
// ZeroPhaseWindowing
//
//////////////////////////////////////////////////////////////////////

/* processing object method implementations */

ZeroPhaseWindowing::ZeroPhaseWindowing(void)
{
	Configure(ZeroPhaseWindowingConfig());
}

ZeroPhaseWindowing::ZeroPhaseWindowing(const ZeroPhaseWindowingConfig &c)
{
	Configure(c);
}

ZeroPhaseWindowing::~ZeroPhaseWindowing()
{
}


/* configure the processing object according to the config object */

bool ZeroPhaseWindowing::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const ZeroPhaseWindowingConfig&>(c);

	return true;
}

/* other functions */



/* setting prototypes for faster processing */

bool ZeroPhaseWindowing::SetPrototypes(){
  return false;}

bool ZeroPhaseWindowing::UnsetPrototypes(){
  return false;}

/* the supervised Do() function */

bool ZeroPhaseWindowing::Do(void)
{
	throw(ErrProcessingObj("ZeroPhaseWindowing::Do(): Supervised mode not implemented"),this);
	return false;
}

/* the  unsupervised Do() function */

bool ZeroPhaseWindowing::Do(TData *pin, TInt32 inSize, TData *pout, TInt32 outSize, TData *pwin, TInt32 winSize)
{
	//decide type of processing: analysis ot synthesis
	switch(mConfig.GetWindowingMethod())
	{
		case 0:      //analysis windowing		
			{
			#ifdef PARANOID
				if(inSize != winSize-1 )
					throw Err("ZeroPhaseWindowing::Do(): window and input buffer are not of the same size");
				if(inSize != outSize )
					throw Err("ZeroPhaseWindowing::Do(): in and out buffer have not the same size");
			#endif
	
				int inMiddle = inSize >> 1; // inMiddle = inSize/2

				for(int i=0,k=inMiddle; i<inMiddle; i++,k++) 
					pout[i] = pin[k] * pwin[k];
					
				int rightEnd = inMiddle << 1;
				for(i=inMiddle+1,k=1; i<rightEnd; i++,k++) // for(i=inMiddle,k=0; i<2*inMiddle; i++,k++)
					pout[i] =  pin[k] * pwin[k];
				
				pout[inMiddle] = 0;
			}
			break;
		case 1:     //synthesis windowing considering overlap add window size
			{
				int overlapSize = mConfig.GetOverlapSize();
				//check if window and in and out buffer are of the same size 
			#ifdef PARANOID
				if(2*overlapSize != outSize )
					throw Err("ZeroPhaseWindowing::Do(): overlapSize and output buffer are not of the same size");
			#endif
				int winMiddle = (winSize-1) >> 1; // TInt32 winMiddle = (winSize-1)/2
				int inMiddle = inSize >> 1; // TInt32 inMiddle = inSize/2
	
				int helpIndex1 = inSize-overlapSize;
				int helpIndex2 = winMiddle-overlapSize;

				for(int i=0, k=helpIndex1; i<overlapSize; k++, i++)
					pout[i] = pin[k] * pwin[helpIndex2+i];

				for(i=0, k=0; i<overlapSize; i++, k++)
					pout[overlapSize+i] = pin[k] * pwin[winMiddle+i];
			}
			break;
	}
	return true;
}


/* overloaded unsupervised Do() function */

bool ZeroPhaseWindowing::Do(Audio &in, Audio &out, DataArray &window)
{
	return Do(in.GetFloatBuffer(), out.GetFloatBuffer(), window);
}

bool ZeroPhaseWindowing::Do(Audio &in, Audio &out, Window &window)
{
	return Do(in.GetFloatBuffer(), out.GetFloatBuffer(), window.GetWindowBuffer());
}

bool ZeroPhaseWindowing::Do(DataArray &in, DataArray &out, Window &window)
{
	return Do(in, out, window.GetWindowBuffer());
}

bool ZeroPhaseWindowing::Do(DataArray &in, DataArray &out, DataArray &window)
{
	return Do(in.GetPtr(), in.Size(), out.GetPtr(), out.Size(), window.GetPtr(), window.Size());
}
