// WindowGen.cpp: implementation of the WindowGen class.
//
//////////////////////////////////////////////////////////////////////

#include "WindowGen.hxx"

using namespace MTG;



/* The  Configuration object has at least to have a name */

void WindowGenConfig::Init()
{
  /* the dynamic type takes care if we add an existing attr .. */
  AddName();
  AddWindowType();
	AddForceZeroPhaseWindow();
	AddInvertWindow();
	AddOverlapWindow();		
	AddFrameSize();

  /* All Attributes are added */
  UpdateData();

  /* Set default values */
  SetName("");
	SetWindowType(MTG::EWindowType::BLACKMAN_HARRIS_62);
  SetForceZeroPhaseWindow(1);
	SetInvertWindow(0);
	SetOverlapWindow(0);
	SetFrameSize(0);
}


/* Processing  object Method  implementations */
WindowGen::WindowGen(void)
{
	Configure(WindowGenConfig());
}

WindowGen::WindowGen(const WindowGenConfig &c)
{
	Configure(c);
}

WindowGen::~WindowGen()
{
}


/* Configure the Processing Object according to the Config object */

bool WindowGen::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
{
	  mConfig = dynamic_cast<const WindowGenConfig&>(c);
	  return true;
}

/* Setting Prototypes for faster processing */

bool WindowGen::SetPrototypes(){
  return false;}

bool WindowGen::UnsetPrototypes(){
  return false;}

/* The supervised Do() function */

bool  WindowGen::Do(void)
{
	throw(ErrProcessingObj("WindowGen::Do(): Supervised mode not implemented"),this);
	return false;
}


/* The  unsupervised Do() function */

bool  WindowGen::Do(TSize	size, Array<TData> &window)
{
	//if ForceZeroPhaseWindow is TRUE change the window size if it is even
	if(mConfig.GetForceZeroPhaseWindow())
	{
		if(size % 2 == 0)
		{
			size += 1;	//add one to change even window length to odd
		}
	}
	//test allocated size for the output
	if(window.Size() != size){
    window.Resize(size);
    window.SetSize(size);
	}
  
  TData *pWindow = window.GetPtr();
	MTG::EWindowType type = mConfig.GetWindowType();
	switch(type)
	{
	case MTG::EWindowType::BLACKMAN_HARRIS_62: 
			BlackmanHarris62(size, pWindow);
			break;
		case MTG::EWindowType::BLACKMAN_HARRIS_70: 
			BlackmanHarris70(size, pWindow);			
			break;
		case MTG::EWindowType::BLACKMAN_HARRIS_74: 
			BlackmanHarris74(size, pWindow);
			break;
		case MTG::EWindowType::BLACKMAN_HARRIS_92: 
			BlackmanHarris92(size, pWindow);
			break;
		case MTG::EWindowType::HAMMING: 
			Hamming(size, pWindow);
			break;
		case MTG::EWindowType::KAISERBESSEL17:
			KaiserBessel(size,pWindow,1.7);
			break;
		case MTG::EWindowType::KAISERBESSEL18:
			KaiserBessel(size,pWindow,1.8);
			break;
		case MTG::EWindowType::KAISERBESSEL19:
			KaiserBessel(size,pWindow,1.9);
			break;
		case MTG::EWindowType::KAISERBESSEL20:
			KaiserBessel(size,pWindow,2.0);
			break;
		case MTG::EWindowType::KAISERBESSEL25:
			KaiserBessel(size,pWindow,2.5);
			break;
		case MTG::EWindowType::KAISERBESSEL30:
			KaiserBessel(size,pWindow,3.0);
			break;
		case MTG::EWindowType::KAISERBESSEL35:
			KaiserBessel(size,pWindow,3.5);
			break;
		case MTG::EWindowType::TRIANGULAR:
			Triangular(size,pWindow);
			break;
		case MTG::EWindowType::CONSTANT_1:
			Constant(size,pWindow);
			break;
		default:
			BlackmanHarris62(size, pWindow);
	};

	if(mConfig.GetInvertWindow())
	{
		pWindow[0] = 0;
		for(int i=1; i<size-1; i++)
		{
			if(pWindow[i] != 0)
				pWindow[i] = 1.0 / pWindow[i];
		}
		pWindow[size-1] = 0;
	}

	//calculate overlap add window 
	if(mConfig.GetOverlapWindow())
	{
		TInt16	overlapSize	= mConfig.GetFrameSize();
		TInt16	overlapWindowSize = 2 * overlapSize;

		double incr = 1.0 / (double)overlapSize;

		TInt32 Size = size-1;
		TData Val = 0;
		if (Size > overlapSize*2)
			for(int i=0; i<Size/2-overlapSize; i++)
				pWindow[i] = 0;
		for (int i=Size/2-overlapSize; i<Size/2; i++)
		{
			pWindow[i] = Val * pWindow[i];
			Val += incr;
		}
		pWindow[Size/2] = pWindow[Size/2];
		Val = 0;
		for (i=Size/2-overlapSize; i<Size/2; i++)
		{
			pWindow[Size - i] = Val * pWindow[Size - i];
			Val += incr;
		}
		if (Size > overlapSize*2)
			for(int i=0; i<Size/2-overlapSize; i++)
				pWindow[Size-i] = 0;
	}	
	return true;
}


/* overloaded Do() function */

bool WindowGen::Do(TSize size, Window  &out)
{
	DataArray	&pwinBuffer = out.GetWindowBuffer();

	//set size in window
	out.SetSize(size);

	//set window type in window
	out.SetWindowType(mConfig.GetWindowType());

	/* call Do() function below */
	Do(size, pwinBuffer);
	return true;
}



double WindowGen::BesselFunction(double X)
{
/* function that returns the zero-order modified Bessel function of the first kind of X
 *
 */
	int i;
	double Sum = 0;
	double Factorial = 1.0;
	double HalfX = X/2.0;
	Sum += 1.0;
	Sum += HalfX * HalfX;
	for(i=2; i<50; i++)
	{
		Factorial *= i;
		Sum += pow( pow(HalfX,i) / Factorial, 2);
	}
	return Sum;
}

void WindowGen::Constant(TSize size,TData *pWindow)
{
	TData fSum = 0;
	for(int i=0; i<size; i++)
		fSum += pWindow[i] = 1.0;
	fSum /= 2;	
	for(i=0; i<size; i++)
		pWindow[i] /= fSum;

}

void WindowGen::KaiserBessel(TSize size,TData *pWindow,double alpha)
{
 /** 
  * function to create a Kaiser-Bessel window
  * int     size;   window size (must be odd)
  * float  *pWindow;      window array
  */
	int     i;
	double fSum = 0;
	double PiAlpha = PI * alpha;
	TSize WindowSize = size;
	double dHalfsize = WindowSize/2.0;
	TInt32 iHalfsize = WindowSize/2;
	// compute window
	if (WindowSize % 2 == 0)
		for(i=0; i<iHalfsize; i++) 
		{
			pWindow[iHalfsize + i] = BesselFunction(PiAlpha * sqrt(1.0 - pow((double)i / dHalfsize, 2))) / BesselFunction(PiAlpha);
			fSum += pWindow[iHalfsize + i];
		}
	else
		for(i=0; i<=iHalfsize; i++) 
		{
			pWindow[iHalfsize + i] = BesselFunction(PiAlpha * sqrt(1.0 - pow((double)i / dHalfsize, 2))) / BesselFunction(PiAlpha);
			fSum += pWindow[iHalfsize + i];
		}
	for(i=0; i<iHalfsize; i++)
	{
		pWindow[i] = pWindow[WindowSize-1-i];
		fSum += pWindow[i];
	}
  // scale function
	fSum = fSum / 2;
	for(i=0; i<WindowSize; i++)
		pWindow[i] = pWindow[i] / fSum;
}


void WindowGen::BlackmanHarris62(TSize size,TData *pWindow)
{
/** function to create a backmanHarris window
 *  int     size;   window size
 *  float  *pWindow;      window array
 */
	int i;
	double fSum = 0;
	/* for 3 term -62.05 */
	float a0 = .44959f, a1 = .49364f, a2 = .05677f; 
	double fConst = TWO_PI / (size-1);
  
	/* compute window */
	for(i = 0; i < size; i++) 
	{
		fSum += pWindow[i] = a0 - a1 * cos(fConst * i) +
			a2 * cos(fConst * 2 * i);
	}

  /* scale function */
	fSum = fSum / 2;
	for(i=0; i<size; i++)
		pWindow[i] /= fSum;
}

void WindowGen::BlackmanHarris70(TSize size,TData *pWindow)
{
/* function to create a backmanHarris window
 * int     size;   window size
 * float  *pWindow;      window array
 */
	int i;
	double fSum = 0;
	/* for 3 term -70.83 */
	float a0 = .42323, a1 = .49755, a2 = .07922;
	double fConst = TWO_PI / (size-1);
  
	/* compute window */
	for(i = 0; i < size; i++) 
	{
		fSum += pWindow[i] = a0 - a1 * cos(fConst * i) +
			a2 * cos(fConst * 2 * i);
	}

 /* scale function */
	fSum = fSum / 2;
	for(i=0; i<size; i++)
		pWindow[i] /= fSum;
}

void WindowGen::BlackmanHarris74(TSize size,TData *pWindow)
{
/* function to create a backmanHarris window
 * int     size;   window size
 * float  *pWindow;      window array
 */
	int     i;
	double fSum = 0;
	/* for -74dB  from the Nuttall paper */
	float a0 = .40217, a1 = .49703, a2 = .09892, a3 = .00188;
	double fConst = TWO_PI / (size-1);
  
	/* compute window */
	for(i = 0; i < size; i++) 
	{
		fSum += pWindow[i] = a0 - a1 * cos(fConst * i) +
			a2 * cos(fConst * 2 * i) - a3 * cos(fConst * 3 * i);
	}

	/* scale function */
	fSum = fSum / 2;
	for(i=0; i<size; i++)
		pWindow[i] /= fSum;
}

void WindowGen::BlackmanHarris92(TSize size,TData *pWindow)
{
/* function to create a backmanHarris window
 * int     size;   window size
 * float  *pWindow;      window array
 */
	int     i;
	double fSum = 0;
	/* for -92dB */
	float a0 = .35875, a1 = .48829, a2 = .14128, a3 = .01168;
	double fConst = TWO_PI / (size-1);
  
	/* compute window */
	for(i = 0; i < size; i++) 
	{
		fSum += pWindow[i] = a0 - a1 * cos(fConst * i) +
			a2 * cos(fConst * 2 * i) - a3 * cos(fConst * 3 * i);
	}

  /* scale function */
	fSum = fSum / 2;
	for(i=0; i<size; i++)
		pWindow[i] /= fSum;
}

void WindowGen::Hamming(TSize size,TData *pWindow)
{
/* function to design a Hamming window
 * int     size;   window size
 * float  *pWindow;      window array
 */
	int     i;
	float fSum = 0;

	for(i = 0; i < size; i++) 
		fSum += pWindow[i] = 0.54 - 0.46*cos(2.0*PI*i/(size-1));
   
  /* scale function */
	fSum = fSum / 2;
	for(i=0; i<size; i++)
		pWindow[i] /= fSum;
}

void WindowGen::Triangular(TSize size,TData *pWindow)
{
/* function to design a Triangular window
 * int     size;   window size
 * float  *pWindow;      window array
 */
	int     i;
	float fSum = 0;
	if (size % 2 == 0)
	{
		for(i = 0; i <= size/2; i++) 
			fSum += pWindow[i] = (float)2*i/(size-1);
		for(i = 0; i < size/2-1; i++)
			fSum += pWindow[size-1-i] = (float)2*i/(size-1);
	}
	else
	{
		size--;
		for(i = 0; i <= size/2; i++) 
			fSum += pWindow[i] = (float)2*i/(size-1);
		for(i = 0; i < size/2; i++)
			fSum += pWindow[size-i] = (float)2*i/(size-1);
	}

  /* scale function */
	fSum = fSum / 2;
	for(i=0; i<size; i++)
		pWindow[i] /= fSum;
}


