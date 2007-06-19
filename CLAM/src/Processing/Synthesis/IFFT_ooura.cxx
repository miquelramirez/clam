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


#include "IFFT_ooura.hxx"
#include "FFT_ooura.hxx"

#include "Assert.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "CLAM_Math.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM {

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, IFFT_ooura> regFFT_ooura("IFFT_ooura");
}

 
  bool IFFT_ooura::ConcreteConfigure(const ProcessingConfig& c) {
		  int oldSize=mSize;
	CopyAsConcreteConfig(mConfig, c);
		if (mConfig.HasAudioSize()) {
			CLAM_ASSERT (mConfig.GetAudioSize()>=0,"Wrong (negative) Size in IFFT Configuration.");
			mSize = mConfig.GetAudioSize();	
			if(mSize>0)
			{
				mOutput.SetSize( mSize );
				mOutput.SetHop( mSize );
			}
		}
		else
			mSize = CLAM_DEFAULT_IFFT_SIZE;

		mState=sOther;
		mComplexflags.bComplex=1;
		mComplexflags.bMagPhase=0;
		if (mSize>0) {
			delete [] ifftbuffer;
			ifftbuffer = new TData[mSize];
	}
		else ifftbuffer = 0;
	
	if ( !isPowerOfTwo( mSize ) )
	{
		AddConfigErrorMessage("Configure failed: IFFT Ooura algorithm only works for input buffers that are a power of two!");
		return false;
	}
	if (mSize>0) {
	  if (mSize != oldSize) {
		ReleaseMemory();
		SetupMemory();
	  }
	  return true;
	}
	ReleaseMemory();
	return false;
  }

  void IFFT_ooura::ReleaseMemory() {
	if (ip) { delete[] ip; ip = 0; }
	if (w) { delete[] w; w = 0; }
  }

  void IFFT_ooura::SetupMemory() {
	int ipSize = (int)(2+(1<<(int)(log(mSize/2+0.5)/log(2.0))/2));
	ip = new int[ipSize];
	for (int i=0; i<ipSize; i++) ip[i] = 0;

	int wSize = (int)(mSize*5/8-1);
	w = new TData[wSize];
	for (int i=0; i<wSize; i++) w[i] = 0;
  }

  IFFT_ooura::IFFT_ooura()
	:  ip(0), w(0)
  {
	Configure(IFFTConfig());
  }

  IFFT_ooura::IFFT_ooura(const IFFTConfig &c) throw(ErrDynamicType)
	: ip(0), w(0)
  { 
	Configure(c);
  };

  IFFT_ooura::~IFFT_ooura()
  {
	ReleaseMemory();
  }

  bool IFFT_ooura::Do()
  {
	bool toReturn = Do(mInput.GetData(),mOutput.GetAudio());
	mInput.Consume();
	mOutput.Produce();
	return toReturn;
  }

  bool IFFT_ooura::Do(const Spectrum& in, Audio &out) const{
	TData* outbuffer = out.GetBuffer().GetPtr();
	int i;

	CLAM_DEBUG_ASSERT(IsRunning(),
					  "IFFT_ooura: Do(): Not in execution mode");
	
	CLAM_DEBUG_ASSERT(isPowerOfTwo(mSize),
					  "IFFT_ooura: Do(): Not a power of two");

	out.SetSampleRate(in.GetSpectralRange()*2);

	TData twoOverSize = 2.0/mSize;

	switch(mState) {
	case sComplex:
	  ComplexToIFFTOoura(in);
	  FFT_ooura::rdft(mSize, -1, ifftbuffer, ip, w);
	  // Buffer dump. This is a kludge; the right way to do this
	  // is using a non-inplace version of rdft (which would
	  // not reduce performance).
	  for (i=0; i<mSize; i++)
		outbuffer[i]=ifftbuffer[i]*twoOverSize;
	  break;
	case sOther:
	  OtherToIFFTOoura(in);
	  FFT_ooura::rdft(mSize, -1, ifftbuffer, ip, w);
		// Buffer dump. This is a kludge; the right way to do this
	  // is using a non-inplace version of rdft (which would
	  // not reduce performance).
	  for (i=0; i<mSize; i++)
		outbuffer[i]=ifftbuffer[i]*twoOverSize;

	  break;
	default:
	  CLAM_ASSERT(false, "IFFT_ooura: Do(): Inconsistent state");
	}

	return true;
  }

  void IFFT_ooura::ComplexToIFFTOoura(const Spectrum &in) const  {
	int i;
	Array<Complex> inbuffer = in.GetComplexArray();

	ifftbuffer[0] = inbuffer[0].Real();
	ifftbuffer[1] = inbuffer[mSize/2].Real();
		
	for (i=1; i<mSize/2; i++) {
	  ifftbuffer[2*i] = inbuffer[i].Real();  
	  ifftbuffer[2*i+1] = -inbuffer[i].Imag();
	}
  }
  
  inline void IFFT_ooura::OtherToIFFTOoura(const Spectrum &in) const
	{
		SpecTypeFlags flags;
		in.GetType(flags);
	/*	if(flags.bMagPhase)
		{
			DataArray& inMag = in.GetMagBuffer();
			DataArray& inPhase = in.GetPhaseBuffer();
			ifftbuffer[0] = inMag[0]*CLAM_cos(inPhase[0]);//inbuffer[0].Real();
			ifftbuffer[1] = inMag[mSize/2]*CLAM_cos(inPhase[mSize/2]);//inbuffer[mSize/2].Real();
			int i;
			TData* ifftbuffer_iter1 = &(ifftbuffer[2]);
			TData* ifftbuffer_iter2 = &(ifftbuffer[3]);
			TData* inMag_iter = &(inMag[1]);
			TData* inPhase_iter = &(inPhase[1]);
			for (i=1; i<mSize/2; ifftbuffer_iter1+=2,ifftbuffer_iter2+=2, inMag_iter++, inPhase_iter++, i++) {
				*ifftbuffer_iter1 = (*inMag_iter)*CLAM_cos((*inPhase_iter));
				*ifftbuffer_iter2 = (*inMag_iter)*CLAM_sin(*inPhase_iter);
			}
		
		}
		else
		{
	*/		Spectrum tmpSpec = in;
			if (!in.HasComplexArray()) {
				flags.bComplex=1;
				tmpSpec.SetTypeSynchronize(flags);
			}
			ComplexToIFFTOoura(tmpSpec);
	//	}

	}
	
	bool IFFT_ooura::SetPrototypes(const Spectrum& in, const Audio &out)
	{
		CheckTypes(in,out);

		SpecTypeFlags flags; 
		in.GetType(flags);

		if (flags.bComplex)
			mState=sComplex;
		else
		{
			CLAM_ASSERT(flags.bPolar || flags.bMagPhase || flags.bMagPhaseBPF,"IFFT_ooura: SetPrototypes(): No Spectrum Attributes!");
			mState=sOther;
		}
		// TODO: Maybe should update the prototype in the port?

		return true;
	}
	
	bool IFFT_ooura::SetPrototypes()
	{
		// @todo Check port prototypes, and set the state (or de
		// backup state if disabled) acordingly.
		CLAM_ASSERT(false,"IFFT_ooura::SetPrototypes: Not implemented.");
		return false;
	}

	bool IFFT_ooura::UnsetPrototypes()
	{
		mState=sOther;
		return true;
	}
	
	void IFFT_ooura::CheckTypes(const Spectrum& in, const Audio &out) const
	{
		CLAM_ASSERT(out.HasBuffer(),"IFFT Do: Float attribute required for Audio object.");
		CLAM_BEGIN_CHECK
			// Input object checking
			if (out.GetSize()!=mSize) { 
				std::stringstream ss;
				ss << "IFFT_ooura::Do: Wrong size in IFFT Audio output\n"
				   << "  Expected: " << mSize*2+1 << ", used " << out.GetSize();
				CLAM_ASSERT(0,ss.str().c_str());
			}
			if (in.GetSize() < mSize/2+1 ) { // ALGORITHM DEPENDENT CHECKING
				std::stringstream ss;
				ss << "IFFT_ooura::Do: not enough memory in input Spectrum.\n"
				   << "  Expected: " << mSize/2+1 << ", used " << in.GetSize();
				CLAM_ASSERT(0,ss.str().c_str());

			}
		CLAM_END_CHECK
	}




}; // CLAM

