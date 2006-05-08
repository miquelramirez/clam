/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <complex>
#include "SimpleOscillator.hxx"
#include "wave_utils.hxx"

using namespace CLAM;

namespace qtvm_examples_utils
{
    void chirp(const double& freq0, const double& freq1, const double& duration, const double& sampleRate, Audio& out)
    {
		const double pi = 3.141592653589793;

		TSize nSamples = TSize(duration*sampleRate);
		out.SetSize(nSamples);
		out.SetSampleRate(TData(sampleRate));
		out.SetBeginTime(0.0);

		std::complex<double> j = std::complex<double>(0.0,1.0);
		double dt = 1.0/sampleRate;
		double mu = (freq1-freq0)/(2.0*duration);
	
		for(TIndex i=0; i < nSamples; i++)
		{
			double psi = 2.0*pi*(freq0*double(i)*dt+mu*double(i)*dt*double(i)*dt);
			out.GetBuffer()[i]=TData(std::exp(j*psi).real());
		}	
    }

    void sine(const TData& freq, const TData& amplitude, const TData& duration, const TData& sampleRate, Audio& out)
    {
		TSize nSamples = TSize(duration*sampleRate);
		TSize frameSize = 512;

		out.SetSize(nSamples);
		out.SetSampleRate(sampleRate);

		SimpleOscillatorConfig oscCfg;
		oscCfg.SetSamplingRate(sampleRate);
		oscCfg.SetFrequency(freq);
		oscCfg.SetAmplitude(TData(amplitude));

		SimpleOscillator osc(oscCfg);
			
		Audio audioFrame;
		audioFrame.SetSize(frameSize);

		osc.Start();

		for(TIndex i=0; i < nSamples; i+=frameSize)
		{
			osc.Do(audioFrame);
			out.SetAudioChunk(i,audioFrame);
		}
		osc.Stop();
    }
}

// END

