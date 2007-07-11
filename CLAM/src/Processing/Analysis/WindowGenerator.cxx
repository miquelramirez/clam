/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ProcessingData.hxx"
#include "DataTypes.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"
#include "WindowGenerator.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
	
namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, WindowGenerator> regWindowGenerator("WindowGenerator");
}

/* Processing  object Method  implementations */

WindowGenerator::WindowGenerator(const WindowGeneratorConfig &c)
	: mOutput( "Generated window function samples", this )
	, mSize("Size",this)
{
	Configure(c);
}

WindowGenerator::~WindowGenerator()
{}


/* Configure the Processing Object according to the Config object */

bool WindowGenerator::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	mSize.DoControl(TControlData(mConfig.GetSize()));

	if (!mConfig.HasUseTable()) return true;
	if (!mConfig.GetUseTable()) return true;


	/* Fill the table */

	mTable.Resize(mConfig.GetSize());
	mTable.SetSize(mConfig.GetSize());
	mSize.DoControl(TControlData(mConfig.GetSize()));

	EWindowType type = mConfig.HasType()?
		mConfig.GetType() :
		EWindowType::eHamming;

	CreateTable(mTable,type,mConfig.GetSize());

	return true;

}

/* The supervised Do() function */

bool  WindowGenerator::Do()
{
	CLAM_ASSERT( AbleToExecute(), "This processing is not ready to do anything" );
	
	bool result = Do( mOutput.GetAudio() );
	mOutput.Produce();
	return result;
}

/* The  unsupervised Do() function */

bool  WindowGenerator::Do(DataArray& out)
{
	const int winsize = (int) mSize.GetLastValue();
	const int audiosize = out.Size();

	bool useTable = mConfig.HasUseTable() && mConfig.GetUseTable();

	if (useTable)
		CreateWindowFromTable(out);
	else {
		EWindowType type = mConfig.HasType()?
			mConfig.GetType() :
			EWindowType::eHamming;
		CreateTable(out,type,winsize);
	}

	//zero padding is applied if audiosize is greater than window size
	if (winsize < audiosize) {
		TData* audio = out.GetPtr();
		memset(audio+winsize,0,(audiosize-winsize)*sizeof(TData));
	}

	NormalizeWindow(out);
	if (mConfig.GetInvert())
		InvertWindow(out);

	return true;
}

bool  WindowGenerator::Do(Audio& out)
{
	Do(out.GetBuffer());

	return true;
}


bool  WindowGenerator::Do(Spectrum& out)
{

	CLAM_ASSERT(out.HasMagBuffer(),
		    "WindowGenerator::Do(): Spectral Window exists only for type MagPhase");

	Do(out.GetMagBuffer());
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
		case EWindowType::eGaussian:
		{
			Gaussian(windowsize,table);
			break;
		}
		case EWindowType::eBlackmanHarrisLike:
		{
			BlackmanHarrisLike(windowsize,table);
			break;
		}
		case EWindowType::eSine:
		{
			Sine(windowsize, table);
			break;
		}
		case EWindowType::eSquare:
		case EWindowType::eNone:
 		{
 			Square(windowsize, table);
 			break;
		}
	}
}

/*Create window from table*/
void WindowGenerator::CreateWindowFromTable(DataArray &array) const
{
	unsigned int index = 0x00000000; 
	unsigned int increment = (unsigned int)((double) (0x00010000) * mConfig.GetSize()/
		(mSize.GetLastValue()));

	//fix point increment [ 16bit | 16bit ] --> 1 = [ 0x0001 | 0x0000 ]

	int size = int(mSize.GetLastValue());
	CLAM_ASSERT(size<=array.Size(),"WindowGenerator::CreateWindowFromTable:output array does not have a valid size");
	for (int i=0;i<size;i++)
	{
		const TData & val = mTable[index>>16];
		array[i] = val;
		index += increment;
	}
}


/* function that returns the zero-order modified Bessel function of the first
kind of X*/

double WindowGenerator::BesselFunction(double x) const
{
	double Sum = 0;
	double Factorial = 1.0;
	double HalfX = x/2.0;
	Sum += 1.0;
	Sum += HalfX * HalfX;
	for(int i=2; i<50; i++)
	{
		Factorial *= i;
		Sum += CLAM_pow( CLAM_pow(HalfX, (double)i) / Factorial, 2.0);
	}
	return Sum;
}

/* function to create a Kaiser-Bessel window; window size (must be odd)*/

void WindowGenerator::KaiserBessel(long size,DataArray& window,
                              double alpha) const
{
	TData PiAlpha = TData(PI) * TData(alpha);
	long windowsize = size;

	TData dHalfsize = windowsize/2.0f;
	int iHalfsize = (int)windowsize/2;

	// compute window
	if (windowsize % 2 != 0)
		window[iHalfsize]= TData(BesselFunction(PiAlpha) / BesselFunction(PiAlpha));
	for(int i=0; i<iHalfsize; i++)
	{
		window[i] = window[windowsize-i-1] =TData(
		   BesselFunction(PiAlpha * CLAM_sqrt(1.0 - CLAM_pow((double)(i-iHalfsize) /
		   dHalfsize, 2.0))) / BesselFunction(PiAlpha) );
	}

}

/* function to create a backmanHarris window*/
void WindowGenerator::BlackmanHarrisX(long size,DataArray& window,
                                 double a0,double a1,double a2,double a3) const
{
	double fConst = TWO_PI / (size-1);
	/* compute window */

	if(size%2 !=0)
	{
		window[(int)(size/2)] = a0 - a1 * CLAM_cos(fConst * ((int)(size/2))) + a2 *
			CLAM_cos(fConst * 2 * ((int)(size/2))) - a3 * CLAM_cos(fConst * 3 * ((int)(size/2)));
	}
	for(int i = 0; i < (int)(size/2); i++)
	{
		window[i] = window[size-i-1] = a0 - a1 * CLAM_cos(fConst * i) +
			a2 * CLAM_cos(fConst * 2 * i) - a3 * CLAM_cos(fConst * 3 * i);
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

void WindowGenerator::BlackmanHarrisLike(long size, DataArray& window) const
{
	TData fSum=0;
//	float a0 = .51f, a1 = .42f, a2 = -0.04f, a3 = .03f, a4=0.03f, a5=0.05f;
	for(int i=0; i<size; i++)
		fSum += window[i] = 
			+ 0.47
			- 0.45 * CLAM_cos(TData(TWO_PI/(size-1.0)*i)) 
			- 0.01 * CLAM_cos(TData(TWO_PI/(size-1.0)*i*2.0)) 
			- 0.01 * CLAM_cos(TData(TWO_PI/(size-1.0)*i*3.0));
	fSum = fSum/2;
	for (int i = 0; i < size; i++)
		window[i] = window[i] / fSum;
	return;
}


/* function to design a Hamming window*/
void WindowGenerator::Hamming(long size,DataArray& window) const
{
	if(size%2 !=0)
		window[(int)(size/2)]= 
			+ TData(0.54)
			- TData(0.46)*CLAM_cos(TData(2.0)*TData(PI)*((int)(size/2))/(size-1));
	for(int i = 0; i < (int)(size/2); i++) 
		window[i] = window[size-i-1] = 
			+ TData(0.54)
			- TData(0.46) * CLAM_cos(TData(2.0)*TData(PI)*i/(size-1));
}

/* function to design a Triangular window*/
void WindowGenerator::Triangular(long size,DataArray& window) const
{
	if(size%2 !=0)
		window[(int)(size/2)] = (TData)2*((int)(size/2))/(size-1);
	for(int i = 0; i < (int)(size/2); i++)
	{
		window[i] = window[size-i-1]= (TData)2*i/(size-1);
	}
}

/* function to create the (fft-)transformed Mainlobe of a BlackmanHarris 92 dB window*/
void WindowGenerator::BlackmanHarris92TransMainLobe(long size,DataArray& window) const
{
	const short N = 512;
	TData fA[4] = {TData(.35875), TData(.48829), TData(.14128), TData(.01168)},
		fMax = 0;
	TData fTheta = -TData(4.0) * TData(TWO_PI) / N,
	       fThetaIncr = (TData(8.0) * TData(TWO_PI) / N) / (size);


	for(int i = 0; i < size; i++)
	{
		window[i] = 0;  // init value
		for (int m = 0; m < 4; m++)
			window[i] +=  -1 * (fA[m]/2) *
				(Sinc (fTheta - m * TData(TWO_PI)/N, N) +
				   Sinc (fTheta + m * TData(TWO_PI)/N, N));
		fTheta += fThetaIncr;
	}

	/* normalize window */
	fMax = window[(int) size / 2];
	for (int i = 0; i < size; i++)
		window[i] = window[i] / fMax;

}

void WindowGenerator::Gaussian(long size,DataArray& window) const
{
	double  s = 0.15;
	double scale = 1.0 / ( 2.0 * M_PI * 0.15 * 0.15 );

	if(size%2 !=0)
		window[size/2] = scale;
	for(int i = 0; i < size/2; i++)
	{
		TData x = (TData)(i-(TData)size/2.)/(TData)(size-1);
		window[i] = window[size-i-1]= scale * exp(-(x*x)/(2*s*s));
	}
}

/* function to design a Sine window*/
void WindowGenerator::Sine(long size,DataArray& window) const
{
	double tmp1 = PI/(2.0*float(size));
	double tmp2 = 0.5*(2.0*float(size));

	for (int i=0;i<size;i++) 
	  window[i] = (float)(1+tmp2*CLAM_sin(tmp1*(i+1)));
}
void WindowGenerator::Square(long size,DataArray& window) const
{
	for (int i=0;i<size;i++) 
	  window[i] = 1;
}


void WindowGenerator::InvertWindow(const DataArray& originalWindow,
		DataArray& invertedWindow) const
{
	if(invertedWindow.AllocatedSize()!=originalWindow.AllocatedSize())
		invertedWindow.Resize(originalWindow.AllocatedSize());
	if(invertedWindow.Size()!=originalWindow.Size())
		invertedWindow.SetSize(originalWindow.Size());

	if (originalWindow.Size()%2!=0)
		if(originalWindow[(int)(originalWindow.Size()/2)]!=0)
			invertedWindow[(int)(originalWindow.Size()/2)]=
				1/originalWindow[(int)(originalWindow.Size()/2)];
	for(int i=0;i<(int)(originalWindow.Size()/2);i++)
	{
		if(originalWindow[i]!=0)
			invertedWindow[i]=invertedWindow[originalWindow.Size()-1-i]=1.0/originalWindow[i];
	}
	invertedWindow[originalWindow.Size()-1]=0;
}

void WindowGenerator::InvertWindow(DataArray& window) const
{
	InvertWindow(window,window);
}

void WindowGenerator::NormalizeWindow(DataArray& window) const
{
	if (mConfig.GetNormalize() == EWindowNormalize::eNone) return;
	double normalizationFactor=1.0;

	const int size = window.Size();
	//Note: We multiply by two because we add the energy of the negative spectrum
	switch (mConfig.GetNormalize()) {
		case EWindowNormalize::eAnalysis:
		{
			double sum=0.0;
			for(int i=0;i<size;i++) sum+=window[i];
			normalizationFactor=1/(sum/2);
			break;
		}
		case EWindowNormalize::eEnergy:
		{
			double sum=0.0;
			for(int i=0;i<size;i++) sum+=window[i];
			normalizationFactor=size/(sum/2);
			break;
		}
		case EWindowNormalize::eMax:
		{
			double max=0.0;
			for(int i=0;i<size;i++)
				if (max<window[i]) max=window[i];
			normalizationFactor = 1.0/max; // be careful with even windows!!
			break;
		}
		default:
			CLAM_ASSERT(false, "Unexpected normalization type");
	}

	for(int i=0;i<size;i++)
	{
		window[i]*=normalizationFactor;
	}
}

/* internal math functions */
 double WindowGenerator::Sinc(double x, short N) const
{
	return (sin ((N/2) * x) / sin (x/2));
}

}

