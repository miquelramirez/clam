#include <complex>
#include "SimpleOscillator.hxx"
#include "wave_utils.hxx"

using namespace CLAM;

namespace qtvm_examples_utils
{
    void chirp(const TData& freq0, const TData& freq1, const TData& duration, const TData& sampleRate, Audio& out)
    {
	const float pi = 3.14159265359;

	TSize nSamples = TSize(duration*sampleRate);
	out.SetSize(nSamples);
	out.SetSampleRate(sampleRate);

	std::complex<float> j = std::complex<float>(0.0f,1.0f);
	float dt = TData(1.0)/sampleRate;
	float mu = (freq1-freq0)/(2.0f*float(duration));
	
	for(TIndex i=0; i < nSamples; i++)
	{
	    float psi = 2.0f*pi*(float(freq0)*float(i)*dt+mu*float(i)*dt*float(i)*dt);
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

