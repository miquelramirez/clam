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

#include "SpectralSynthesis.hxx"
#include "ProcessingFactory.hxx"

#include <iostream>

namespace CLAM
{

namespace Hidden 
{
	static const char * metadata[] = {
		"key", "SpectralSynthesis",
		"category", "Synthesis",
		"description", "SpectralSynthesis",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SpectralSynthesis> regSpectralSynthesis("SpectralSynthesis");
	static FactoryRegistrator<ProcessingFactory, SpectralSynthesis> reg = metadata;
}

void SpectralSynthesis::AttachChildren()
{
	mPO_AnalWindowGen.SetParent(this);
	mPO_SynthWindowGen.SetParent(this);
	mPO_AudioProduct.SetParent(this);
	mPO_CircularShift.SetParent(this);
	mPO_IFFT.SetParent(this);
	
}

SpectralSynthesis::SpectralSynthesis()
	:  mInput("Input",this),
	   mOutput("Output",this)
{
	Configure(SpectralSynthesisConfig());
	AttachChildren();
}

SpectralSynthesis::SpectralSynthesis(const SpectralSynthesisConfig& cfg)
	: mInput("Input",this),
	  mOutput("Output",this)
{
	Configure(cfg);
	AttachChildren();
}

SpectralSynthesis::~SpectralSynthesis()
{
	
}

bool SpectralSynthesis::ConfigureChildren()
{
	
	//instantiate analysis window generator
	if(!mPO_AnalWindowGen.Configure(mConfig.GetAnalWindowGenerator()))
		return false;

	//instantiate synthesis window generator
	if(!mPO_SynthWindowGen.Configure(mConfig.GetSynthWindowGenerator()))
		return false;
	
	//Instantiate Circular Shift
	if(!mPO_CircularShift.Configure(mConfig.GetCircularShift()))
		return false;		


	//instantiate IFFT
	IFFTConfig IFFTCFG;
	IFFTCFG.SetAudioSize(mConfig.GetIFFT().GetAudioSize());
	if(!mPO_IFFT.Configure(IFFTCFG))
		return false;		
		
	return true;
}

void SpectralSynthesis::ConfigureData()
{
	///////////////////////////////////
	//INSTANIATE PROCESSING DATA
	///////////////////////////////////

	//intantiate audio used as temporary data
	mAudio0.SetSize(mConfig.GetIFFT().GetAudioSize());//audio used as output of the IFFT

	mAudio1.SetSize(mConfig.GetAnalWindowSize()-1);//audio without zeropadding
	
	mAudio2.SetSize(mConfig.GetHopSize()*2);//audio used as input of the inverse + triangular windowing 
	
	mSynthWindow.SetSize(mConfig.GetHopSize()*2+1);

	//initialize window
	Audio tmpWindow,tmpWindow2;
	
	tmpWindow.SetSize(mConfig.GetAnalWindowSize());
	tmpWindow2.SetSize(mConfig.GetHopSize()*2+1);

	mPO_AnalWindowGen.Start();
	mPO_AnalWindowGen.Do(tmpWindow);
	mPO_AnalWindowGen.Stop();


	//We now take only the mSynthWindowSize central samples of the inverse window
	tmpWindow.GetAudioChunk((int)((TData)mConfig.GetAnalWindowSize()/2-(TData)mConfig.GetHopSize()),(int)((float)mConfig.GetAnalWindowSize()/2+(float)mConfig.GetHopSize()),mSynthWindow,false);



	//Now we generate triangular synthesis window
	tmpWindow.SetSize(mConfig.GetHopSize()*2+1);

	mPO_SynthWindowGen.Start();
	mPO_SynthWindowGen.Do(tmpWindow);
	mPO_SynthWindowGen.Stop();

	
	//Now we multiply both windows
	mPO_AudioProduct.Do(tmpWindow,mSynthWindow,mSynthWindow);

	//now we set it to even size leaving last sample out
	mSynthWindow.SetSize(mConfig.GetHopSize()*2);



}


bool SpectralSynthesis::ConcreteConfigure(const ProcessingConfig& c)
{
         CopyAsConcreteConfig(mConfig, c);
	 
	 mConfig.Sync();

	 mOutput.SetSize( mConfig.GetHopSize()*2 );
	 mOutput.SetHop( mConfig.GetHopSize()*2 );


	 //CONFIGURE CHILDREN AND DATA
	ConfigureChildren();

	ConfigureData();
	return true;
}

bool SpectralSynthesis::Do(void)
{
	bool result = Do(mInput.GetData(),mOutput.GetAudio());
	mInput.Consume();
	mOutput.Produce();
	return result;
}


bool SpectralSynthesis::Do(Spectrum& in, Audio& out)
{

	TSize analWindowSize = mConfig.GetAnalWindowSize()-1;//note I include the minus 1!
	TSize hopSize = mConfig.GetHopSize();
//Now we do the inverse FFT
	mPO_IFFT.Do(in, mAudio0);
//Undoing Synthesis circular shift
	mPO_CircularShift.Do(mAudio0,mAudio0);
//We take the central samples to multiply with the window while also undoing the zero padding
	int centerSample = analWindowSize*0.5;
	mAudio0.GetAudioChunk(centerSample-hopSize,centerSample+hopSize-1,mAudio2,false);
//Aplying inverse window
	mPO_AudioProduct.Do(mAudio2, mSynthWindow,out);

	
	return true;
}


bool SpectralSynthesis::Do(Frame& in)
{
	if(mConfig.GetResidual())
		return Do(in.GetSpectrum(),in.GetAudioFrame());
	else
		return Do(in.GetResidualSpec(),in.GetResidualAudioFrame());
}

bool SpectralSynthesis::Do(Segment& in)
{
	return Do(in.GetFrame(in.mCurrentFrameIndex++));
}

TInt32 SpectralSynthesis::CalculatePowerOfTwo(TInt32 size)
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


}

