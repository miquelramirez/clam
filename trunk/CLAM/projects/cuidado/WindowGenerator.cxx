#include "ProcessingObject.hxx"
#include "ProcessingData.hxx"
#include "Port.hxx"
#include "DataTypes.hxx"
#include "Enum.hxx"
#include "Array.hxx"
#include "ErrProcessingObj.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"
#include "WindowGenerator.hxx"

#define CLASS "WindowGenerator"

namespace MTG {


	Enum::tEnumValue EWindowType::sEnumValues[] = {
	  {EWindowType::eKaiserBessel17,"KaiserBessel17"},
	  {EWindowType::eKaiserBessel18,"KaiserBessel18"},
	  {EWindowType::eKaiserBessel19,"KaiserBessel19"},
	  {EWindowType::eKaiserBessel20,"KaiserBessel20"},
	  {EWindowType::eKaiserBessel25,"KaiserBessel25"},
	  {EWindowType::eKaiserBessel30,"KaiserBessel30"},
	  {EWindowType::eKaiserBessel35,"KaiserBessel35"},
	  {EWindowType::eBlackmanHarris74,"BlackmanHarris74"},
	  {EWindowType::eBlackmanHarris62,"BlackmanHarris62"},
	  {EWindowType::eBlackmanHarris70,"BlackmanHarris70"},
	  {EWindowType::eBlackmanHarris92,"BlackmanHarris92"},
	  {EWindowType::eHamming,"Hamming"},
	  {EWindowType::eTriangular,"Triangular"},
	  {EWindowType::eBlackmanHarris92TransMainLobe,"BlackmanHarris92 MainLobe"},
		{0,NULL}
	};

	Enum::tValue EWindowType::sDefault = EWindowType::eHamming;

	Enum::tEnumValue EWindowNormalize::sEnumValues[] = {
	  {EWindowNormalize::eNone,"No Normalization"},
	  {EWindowNormalize::eAnalysis,"Normalization for Analysis"},
	  {EWindowNormalize::eEnergy,"Normalization for constant Energy"},
	  {0,NULL}
	};

	Enum::tValue EWindowNormalize::sDefault = EWindowNormalize::eAnalysis;


  void WindowGeneratorConfig::Init()
	{
       	     /* the dynamic type takes care if we add an existing attr .. */

	     AddName();
	     AddSize();
	     AddNormalize();
	 
             /* All Attributes are added */
	     UpdateData();
	     SetSize(1025);
	     SetNormalize(EWindowNormalize::eAnalysis);
	}


  /* Processing  object Method  implementations */

  WindowGenerator::WindowGenerator():
    mSize("Size",this)
	  
	{
		Configure(WindowGeneratorConfig());
	}

  WindowGenerator::WindowGenerator(const WindowGeneratorConfig &c) :
    mSize("Size",this)
	{
		Configure(c);
	}

	WindowGenerator::~WindowGenerator()
	{}


  /* Configure the Processing Object according to the Config object */

	bool WindowGenerator::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
	    mConfig = dynamic_cast<const WindowGeneratorConfig&>(c);	    
	    mSize.DoControl(mConfig.GetSize());

	    if (mConfig.HasUseTable())
	      if (!mConfig.GetUseTable()) return true;


	    /* Fill the table */

			if (mConfig.GetSize()%2 == 0) // We force the size to odd
				mConfig.SetSize(mConfig.GetSize()+1);

	    mTable.SetSize(mConfig.GetSize());
	    mTable.Resize(mConfig.GetSize());

	    EWindowType type;
	    if (mConfig.HasType())
	      type = mConfig.GetType();
	    else type = EWindowType::eHamming;

	    CreateTable(mTable,type,mConfig.GetSize());

	    return true;

	}

  /* Setting Prototypes for faster processing */

        bool WindowGenerator::SetPrototypes(const DataArray& out)
        {
	  return false;
	}
  
        bool WindowGenerator::SetPrototypes()
        {
	  return false;
	}

        bool WindowGenerator::UnsetPrototypes()
        {
	  return false;
	}

  /* The supervised Do() function */

        bool  WindowGenerator::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
  
  /* The  unsupervised Do() function */

	bool  WindowGenerator::Do(DataArray& out)
	{	  
		bool useTable;
		int winsize = (int) mSize.GetLastValue();
		int audiosize = out.Size();
		
		out.SetSize(winsize);

		if (mConfig.HasUseTable())
		 	useTable = mConfig.GetUseTable();
	  else  useTable = true;
	  
	  if (!useTable) {
	    EWindowType type;
	    if (mConfig.HasType())
	      type = mConfig.GetType();
	    else type = EWindowType::eHamming;
	    CreateTable(out,type,int(mSize.GetLastValue()));
	  }
	  else {
	    CreateWindowFromTable(out);
	  }
		
	  if (winsize < audiosize) {
			int i;
			TData* audio = out.GetPtr();
			for (i=winsize;i<audiosize;i++)
				audio[i] = 0.0;
		}
		out.SetSize(audiosize);		
	  
		NormalizeWindow(out);

	  return true;
	}
	
	bool  WindowGenerator::WindowGenerator::Do(Audio& out)
	{
		if (!out.GetType().bFloat)
			throw(ErrProcessingObj(CLASS"::Do(): invalid Audio Data Format"),this);

		//		if (out.GetSize() < mConfig.GetSize())
		//				throw(ErrProcessingObj(CLASS"::Do(): invalid Audio Data Format"),this);

		Do(out.GetFloatBuffer());
		
		return true;
	} 


	bool  WindowGenerator::WindowGenerator::Do(Spectrum& out)
	{

	  if (out.GetType().bMagPhase)	       
	    Do(out.GetMagBuffer());
	  else  
	    throw(ErrProcessingObj(CLASS"::Do(): Spectral Window exist only for type MagPhase"),this);
	  
	  return true;
	} 
	 
	/*Create Table or window 'on the fly'*/
	void WindowGenerator::CreateTable(DataArray& table,EWindowType windowType,
	 																	long windowsize) const
	{ 
		switch(windowType)//use mathematical function according to type
			{
			case EWindowType::eKaiserBessel17:
				{
				 	KaiserBessel(windowsize,table,1.7);
					break;
				}
			case EWindowType::eKaiserBessel18:
				{
					KaiserBessel(windowsize,table,1.8);
					break;
				}
			case EWindowType::eKaiserBessel19:
				{
					KaiserBessel(windowsize,table,1.9);
					break;
				}
			case EWindowType::eKaiserBessel20:
				{
					KaiserBessel(windowsize,table,2.0);
					break;
				}
			case EWindowType::eKaiserBessel25:
				{
					KaiserBessel(windowsize,table,2.5);
					break;
				}
			case EWindowType::eKaiserBessel30:
				{
					KaiserBessel(windowsize,table,3.0);
					break;
				}
			case EWindowType::eKaiserBessel35:
				{
					KaiserBessel(windowsize,table,3.5);
					break;
				}
			case EWindowType::eBlackmanHarris62:
				{
					BlackmanHarris62(windowsize,table);
					break;
				}
			case EWindowType::eBlackmanHarris70:
				{
					BlackmanHarris70(windowsize,table);
					break;
				}
			case EWindowType::eBlackmanHarris74:
				{
					BlackmanHarris74(windowsize,table);
					break;
				}
			case EWindowType::eBlackmanHarris92:
				{
					BlackmanHarris92(windowsize,table);
					break;
				}
			case EWindowType::eHamming:
				{
					Hamming(windowsize,table);
					break;
				}
			case EWindowType::eTriangular:
				{
					Triangular(windowsize,table);
					break;
				}
			case EWindowType::eBlackmanHarris92TransMainLobe:
				{
					BlackmanHarris92TransMainLobe(windowsize,table);
					break;
				}
				
			}
	}
	
	/*Create window from table*/
void WindowGenerator::CreateWindowFromTable(DataArray &array) const
{
  int i;
	unsigned int index = 0x00000000; 
	unsigned int increment = (unsigned int)((double) (0x00010000) * mConfig.GetSize()/
																					(mSize.GetLastValue()));

	//fix point increment [ 16bit | 16bit ] --> 1 = [ 0x0001 | 0x0000 ]

	int size = int(mSize.GetLastValue());
	for (i=0;i<size;i++)
    {
			TData val = mTable[index>>16];      
			array[i] = val;
      index += increment;
    }
}


/* function that returns the zero-order modified Bessel function of the first 
kind of X*/

double WindowGenerator::BesselFunction(double x) const
{
	int i;
	double Sum = 0;
	double Factorial = 1.0;
	double HalfX = x/2.0;
	Sum += 1.0;
	Sum += HalfX * HalfX;
	for(i=2; i<50; i++)
	{
		Factorial *= i;
		Sum += pow( pow(HalfX,i) / Factorial, 2);
	}
	return Sum;
}

/* function to create a Kaiser-Bessel window; window size (must be odd)*/
 
void WindowGenerator::KaiserBessel(long size,DataArray& window,
                              double alpha) const
{
	int     i;

  double PiAlpha = PI * alpha;
	long windowsize = size;

	double dHalfsize = windowsize/2.0;
	int iHalfsize = (int)windowsize/2;

	// compute window
	if (windowsize % 2 != 0)
    window[iHalfsize]= BesselFunction(PiAlpha) / BesselFunction(PiAlpha);
	for(i=0; i<iHalfsize; i++) 
	{
		window[i] = window[windowsize-i-1] =
       BesselFunction(PiAlpha * sqrt(1.0 - pow((double)(i-iHalfsize) / 
       dHalfsize, 2))) / BesselFunction(PiAlpha);
	}
	  
}

/* function to create a backmanHarris window*/
void WindowGenerator::BlackmanHarrisX(long size,DataArray& window,
                                 double a0,double a1,double a2,double a3) const
{
  int i;
  double fConst = TWO_PI / (size-1);
  /* compute window */

	if(size%2 !=0)
  {
    window[(int)(size/2)] = a0 - a1 * cos(fConst * ((int)(size/2))) + a2 * 
      cos(fConst * 2 * ((int)(size/2))) - a3 * cos(fConst * 3 * ((int)(size/2)));
  }
  for(i = 0; i < (int)(size/2); i++) 
	{
		window[i] = window[size-i-1] = a0 - a1 * cos(fConst * i) +
			a2 * cos(fConst * 2 * i) - a3 * cos(fConst * 3 * i);
	}


}

/* function to create a BlackmanHarris window*/
void WindowGenerator::BlackmanHarris62(long size,DataArray& window) const
{
	/* for 3 term -62.05 */
	double a0 = .44959, a1 = .49364, a2 = .05677; 
	BlackmanHarrisX(size,window,a0,a1,a2);
	
}


/* function to create a backmanHarris window*/
 
void WindowGenerator::BlackmanHarris70(long size,DataArray& window) const
{
	/* for 3 term -70.83 */
	double a0 = .42323, a1 = .49755, a2 = .07922;
	BlackmanHarrisX(size,window,a0,a1,a2);
}

/* function to create a backmanHarris window*/
void WindowGenerator::BlackmanHarris74(long size,DataArray& window) const
{

	/* for -74dB  from the Nuttall paper */
	double a0 = .40217, a1 = .49703, a2 = .09892, a3 = .00188;

  BlackmanHarrisX(size,window,a0,a1,a2,a3);
}

/* function to create a backmanHarris window*/
void WindowGenerator::BlackmanHarris92(long size,DataArray& window) const
{

	/* for -92dB */
	double a0 = .35875, a1 = .48829, a2 = .14128, a3 = .01168;
  
	BlackmanHarrisX(size,window,a0,a1,a2,a3);
}

/* function to design a Hamming window*/
void WindowGenerator::Hamming(long size,DataArray& window) const
{
	int     i;
	
	if(size%2 !=0)
    window[(int)(size/2)]= 0.54 - 0.46*cos(2.0*PI*((int)(size/2))/(size-1));
  for(i = 0; i < (int)(size/2); i++) 
		window[i] = window[size-i-1] = 0.54 - 0.46*cos(2.0*PI*i/(size-1));
}

/* function to design a Triangular window*/
void WindowGenerator::Triangular(long size,DataArray& window) const
{
	int     i;
	
  if(size%2 !=0)
    window[(int)(size/2)] = (float)2*((int)(size/2))/(size-1);
	for(i = 0; i < (int)(size/2); i++)
  {
		window[i] = window[size-i-1]= (float)2*i/(size-1);
  }
}

/* function to create the (fft-)transformed Mainlobe of a BlackmanHarris 92 dB window*/
void WindowGenerator::BlackmanHarris92TransMainLobe(long size,DataArray& window) const
{
  short N = 512, i, m;
	float fA[4] = {.35875, .48829, .14128, .01168},
		fMax = 0;
	double fTheta = -4.0 * TWO_PI / N, 
	       fThetaIncr = (8.0 * TWO_PI / N) / (size);


	for(i = 0; i < size; i++) 
	{
		window[i] = 0;  // init value
		for (m = 0; m < 4; m++)
			window[i] +=  -1 * (fA[m]/2) * 
				(Sinc (fTheta - m * TWO_PI/N, N) + 
			     Sinc (fTheta + m * TWO_PI/N, N));
		fTheta += fThetaIncr;
	}
	
	/* normalize window */
	fMax = window[(int) size / 2];
	for (i = 0; i < size; i++) 
		window[i] = window[i] / fMax;

}


void WindowGenerator::InvertWindow(const DataArray& originalWindow,
    DataArray& invertedWindow) const
{
  int i;
  
  if(invertedWindow.AllocatedSize()!=originalWindow.AllocatedSize())
    invertedWindow.Resize(originalWindow.AllocatedSize());
  if(invertedWindow.Size()!=originalWindow.Size())
    invertedWindow.SetSize(originalWindow.Size());
  
  if (originalWindow.Size()%2!=0)
    invertedWindow[(int)(originalWindow.Size()/2)]=
      1/originalWindow[(int)(originalWindow.Size()/2)];
  for(i=0;i<(int)(originalWindow.Size()/2);i++)
  {
    invertedWindow[i]=invertedWindow[originalWindow.Size()-i]=1/originalWindow[i];
  }
}

void WindowGenerator::InvertWindow(DataArray& window) const
{
  InvertWindow(window,window);
}

void WindowGenerator::NormalizeWindow(DataArray& window) const
{
	int i;
	double sum=0.0,invSum;

	if (mConfig.GetNormalize() == EWindowNormalize::eNone) return;
	else {
		int size = window.Size();
		for(i=0;i<(int)size;i++)
			{
				sum+=window[i];
			}
		
		if (mConfig.GetNormalize() == EWindowNormalize::eAnalysis) 
			invSum=1/sum;
		
		if (mConfig.GetNormalize() == EWindowNormalize::eEnergy) 
			invSum=size/sum;
		
		for(i=0;i<(int)size;i++)
			{
				window[i]*=invSum;
      }
  } 
} 

/* internal math functions */
 double WindowGenerator::Sinc(double x, short N) const
{
	return (sin ((N/2) * x) / sin (x/2));
}








};//namespace MTG
