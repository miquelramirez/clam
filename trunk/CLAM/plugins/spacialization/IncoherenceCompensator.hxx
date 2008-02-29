/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef IncoherenceCompensator_hxx
#define IncoherenceCompensator_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MagPhaseSpectrum.hxx"
#include "MyFFT.hxx"
#include "Complex2MagPhaseSpectrum.hxx"
#include <iostream>
namespace CLAM
{

/**
 * Generates a spectrum representing a frequency domain filter 
 * that compensates the frequency magnitude decay produced by 
 * placing several coherent sources with an error on the distance
 * characterized by its standard deviation.
 *
 * To get an idea of why this is needed, given a displacement 
 * between two speakers reproducing the same or similar wave
 * smaller error is required to cancel each other for high
 * frequencys than for lower frequencies.
 *
 * @todo IncoherenceCompensator: Write down the formula and the statistical justification.
 *
 */

class IncoherenceCompensator : public Processing
{ 
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);
	    DYN_ATTRIBUTE( 1, public, CLAM::TData, DistanceStandardDeviation);
	    DYN_ATTRIBUTE( 2, public, CLAM::TData, SampleRate);

	protected:
	    void DefaultInit()
	    {
		AddAll();
		UpdateData();
		SetFrameSize(512);
		SetDistanceStandardDeviation(4.);
		SetSampleRate(44100);
	    };
	};

	OutPort<ComplexSpectrum> mOutComplex;
	OutPort<MagPhaseSpectrum> mOut;
	MagPhaseSpectrum mSpectrum;
	ComplexSpectrum mComplexSpectrum;
	Config mConfig;
public:
	const char* GetClassName() const { return "IncoherenceCompensator"; }
	IncoherenceCompensator(const Config& config = Config()) 
		: mOutComplex("Complex Spectrum", this) 
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return mConfig;
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(mConfig, config);
		if (!FillSpectrum()) return false;
		return true; 
	}

	bool FillSpectrum()
	{
		unsigned frameSize = mConfig.GetFrameSize();
		mComplexSpectrum.bins.resize(frameSize);
		double c = 443;
		double spectralRange = mConfig.GetSampleRate()/2;
		double a = mConfig.GetDistanceStandardDeviation()*M_PI*spectralRange/frameSize/180/c;
		for (unsigned i=0; i<frameSize; i++)
			mComplexSpectrum.bins[i] = exp(a*i);
		return true;
	}
 
	bool Do()
	{
		MagPhaseSpectrum & spectrum = mOut.GetData();
		ComplexSpectrum & complexSpectrum = mOutComplex.GetData();
		if (mSpectrum.magnitudes.size() != !spectrum.magnitudes.size())
			spectrum = mSpectrum;
		if (mComplexSpectrum.bins.size() != !complexSpectrum.bins.size())
			complexSpectrum = mComplexSpectrum;

		// Tell the ports this is done
		mOut.Produce();
		mOutComplex.Produce();
		return true;
	}

};

}

#endif // IncoherenceCompensator_hxx
