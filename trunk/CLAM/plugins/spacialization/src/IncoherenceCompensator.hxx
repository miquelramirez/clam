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
	    /// Standard deviation of the sources distance in centimeters
	    DYN_ATTRIBUTE( 1, public, CLAM::TData, DistanceStandardDeviation);
	    DYN_ATTRIBUTE( 2, public, CLAM::TData, SampleRate);

	protected:
	    void DefaultInit()
	    {
		AddAll();
		UpdateData();
		SetFrameSize(512);
		SetDistanceStandardDeviation(.01);
		SetSampleRate(44100);
	    };
	};

	OutPort<ComplexSpectrum> mOutComplex;
	InControl mDeviation;
	MagPhaseSpectrum mSpectrum;
	ComplexSpectrum mComplexSpectrum;
	Config mConfig;
public:
	const char* GetClassName() const { return "IncoherenceCompensator"; }
	IncoherenceCompensator(const Config& config = Config()) 
		: mOutComplex("Complex Spectrum", this) 
		, mDeviation("Deviation", this)
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
		mDeviation.DoControl(mConfig.GetDistanceStandardDeviation());
		return true; 
	}

	void ComputeSpectralProfile(ComplexSpectrum & output)
	{
		double c = 340;

		double spectralRange = mConfig.GetSampleRate()/2;
		output.spectralRange = spectralRange;

		unsigned frameSize = mConfig.GetFrameSize();
		if (output.bins.size() != frameSize)
			output.bins.resize(frameSize);

		double deviationInCM = mDeviation.GetLastValue();
		// exp( (sigma*w)^2/2); sigma=d/c; w=i*spectralRange*2*pi/nBins
		// 1/100: centimetres -> meters
		// 1/c: meters -> segs
		// spectralRange: segs -> periods
		// 2*M_PI: periods -> radians
		// 1/framesize: samples -> frame
		// a = sigma*w/i
//		std::cout << deviationInCM << std::endl;
		double a = deviationInCM*M_PI*spectralRange/frameSize/180./100./c;

		for (unsigned i=0; i<frameSize; i++)
			output.bins[i] = exp(a*a*i*i/2);
	}

	bool Do()
	{
		ComputeSpectralProfile( mOutComplex.GetData() );
		// Tell the ports this is done
		mOutComplex.Produce();
		return true;
	}

};

}

#endif // IncoherenceCompensator_hxx
