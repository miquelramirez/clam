/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

using namespace CLAM;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*					SpectralSYNTHESIS CONFIGURATION						*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


void SpectralSynthesisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SpectralSynthesisConfig::DefaultValues()
{
/** FFTSize will be next power of two of the window size*/
	SetprZeroPadding(0);
	SetSamplingRate(44100);

	/** Default analysis window size corresponds to 512*/
	GetAnalWindowGenerator().SetName("AnalysisWG");
	SetAnalWindowSize(513);
	SetAnalWindowType(EWindowType::eBlackmanHarris92);
	GetAnalWindowGenerator().SetInvert(true);

	/** WindowSize/2*/
	SetHopSize((GetAnalWindowSize()-1)/2);

	/** Synthesis Window Type is set to triangular*/
	GetSynthWindowGenerator().SetName("SynthesisWG");
	GetSynthWindowGenerator().SetType(EWindowType::eTriangular);
	GetSynthWindowGenerator().SetNormalize(EWindowNormalize::eNone);
	GetSynthWindowGenerator().SetSize(GetHopSize()*2+1);

	SetResidual(false);
	

}

void SpectralSynthesisConfig::SetAnalWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetAnalWindowGenerator().SetSize(w);
	TData audioSize=TData(PowerOfTwo((w-1)*int(pow(TData(2.0),TData(GetZeroPadding())))));
	GetIFFT().SetAudioSize(int(audioSize));
	GetCircularShift().SetAmount(TData(w/2)); 
}

TSize SpectralSynthesisConfig::GetAnalWindowSize() const
{
	return GetAnalWindowGenerator().GetSize();
}

void SpectralSynthesisConfig::SetAnalWindowType(const EWindowType& t)
{
	GetAnalWindowGenerator().SetType(t);
}

const EWindowType& SpectralSynthesisConfig::GetAnalWindowType() const
{
	return GetAnalWindowGenerator().GetType();
}


void SpectralSynthesisConfig::SetSynthWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetSynthWindowGenerator().SetSize(w);
}

TSize SpectralSynthesisConfig::GetSynthWindowSize() const
{
	return GetSynthWindowGenerator().GetSize();
}

void SpectralSynthesisConfig::SetZeroPadding(int z)
{
	SetprZeroPadding(z);
	TData audioSize=TData(PowerOfTwo((GetAnalWindowSize()-1)*int(pow(TData(2),TData(GetZeroPadding())))));
	GetIFFT().SetAudioSize(int(audioSize));
}

int SpectralSynthesisConfig::GetZeroPadding() const
{
	return GetprZeroPadding();
}

void SpectralSynthesisConfig::SetHopSize(TSize h)
{
	GetSynthWindowGenerator().SetSize(2*h+1);
	//GetOverlapAdd().SetHopSize(h);
	//GetOverlapAdd().SetBufferSize(GetFrameSize()+h);
}

TSize SpectralSynthesisConfig::GetHopSize() const
{
	return (GetSynthWindowGenerator().GetSize()-1)>>1;
}

void SpectralSynthesisConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate(int(sr));

	/** for sychronizing existing config*/
	SetAnalWindowSize(GetAnalWindowSize());
}

TData SpectralSynthesisConfig::GetSamplingRate() const
{
	return TData(GetprSamplingRate());
}


TInt32 SpectralSynthesisConfig::PowerOfTwo(TInt32 size)
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

//methods for PO

void SpectralSynthesis::AttachChildren()
{
	mPO_AnalWindowGen.SetParent(this);
	mPO_SynthWindowGen.SetParent(this);
	mPO_AudioProduct.SetParent(this);
	mPO_CircularShift.SetParent(this);
	mPO_IFFT.SetParent(this);
	
}

SpectralSynthesis::SpectralSynthesis():mInput("Input",this,1),
		mOutput("Output",this,1)
{
	Configure(SpectralSynthesisConfig());
	AttachChildren();
}

SpectralSynthesis::SpectralSynthesis(const SpectralSynthesisConfig& cfg):mInput("Input",this,1),
		mOutput("Output",this,1)
{
	Configure(cfg);
	AttachChildren();
}

SpectralSynthesis::~SpectralSynthesis()
{
	
}

void SpectralSynthesis::Attach(Spectrum& in, Audio &out)
{
	mInput.Attach(in);
	mOutput.Attach(out);
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
	IFFTCFG.SetName("ifft_name");
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

	//CONFIGURE CHILDREN AND DATA
	ConfigureChildren();

	ConfigureData();
	return true;
}

bool SpectralSynthesis::Do(void)
{
	return Do(mInput.GetData(),mOutput.GetData());
}


bool SpectralSynthesis::Do(Spectrum& in, Audio& out)
{

	SpecTypeFlags tmpFlags;
	in.GetType(tmpFlags);
	if(!tmpFlags.bComplex)
	{
		tmpFlags.bComplex=1;
	}
//convert MagPhase data to ComplexData
	in.SetTypeSynchronize(tmpFlags); 
//Now we do the inverse FFT
	mPO_IFFT.Do(in, mAudio0);
//Undoing Synthesis circular shift
	mPO_CircularShift.Do(mAudio0,mAudio0);
//Undoing zero padding by hand seems a bit ugly but...
	mAudio0.GetAudioChunk(0,mConfig.GetAnalWindowSize()-1,mAudio1,false);
//Now we take the central samples to multiply with the window
	int centerSample=mAudio1.GetSize()/2;
	mAudio1.GetAudioChunk(centerSample-mConfig.GetHopSize(),centerSample+mConfig.GetHopSize()-1,mAudio2,false);
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
