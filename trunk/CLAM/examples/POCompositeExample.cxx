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

#include "ProcessingComposite.hxx"
#include "FFT.hxx"
#include "IFFT.hxx"
#include "SpectrumProduct.hxx"

using namespace CLAM;

/** Big configuration class */
class BigConfiguration : public ProcessingConfig {
public:
	DYNAMIC_TYPE_USING_INTERFACE (BigConfiguration, 2,ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, int, AudioSize);

	void DefaultInit();
};

/** This is the processing object class we are using to group several
 *  other processing objects. 
 */
class BigProcessing : public ProcessingComposite {

	// Configuration data
	BigConfiguration mConfig;

	// We define the internal data in our system.
	Spectrum ImpulseResponse;
	Spectrum SpectralInput;
	Spectrum SpectralOutput;

	// The internal Processing Objects
	FFT InputFFT;
	SpectrumProduct Filter;
	IFFT OutputIFFT;

	// And the interfaces with the outside world.

	/** Global input port */
	AudioInPort InputAudio;
	/** Global output port */
	AudioInPort OutputAudio;

	// Internal convenience methods.

	void AdoptChildren();
	void AttachChildren();
	bool ConfigureChildren(int size);
	void ConfigureData(int size);

	// Processing Object compliance methods.

	const char *GetClassName() const {return "BigProcessing";}

	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

public:
	BigProcessing(BigConfiguration&);
	BigProcessing();

	const ProcessingConfig &GetConfig() const {return mConfig;}

	/** Supervised mode execution */
	bool Do(void);

	/** Unsupervised mode execution */
	bool Do(Audio& in, Audio &out);

};

void BigProcessing::AdoptChildren()
{
	InputFFT.SetParent(this);
	Filter.SetParent(this);
	OutputIFFT.SetParent(this);
}

BigProcessing::BigProcessing() :
	InputAudio("Input Audio",this),
	OutputAudio("Output Audio",this)
{
	AdoptChildren();
	Configure(BigConfiguration());
}

BigProcessing::BigProcessing(BigConfiguration& cfg) :
	InputAudio("Input Audio",this),
	OutputAudio("Output Audio",this)
{
	AdoptChildren();
	Configure(cfg);
}

bool BigProcessing::ConfigureChildren(int size)
{
	// Unused variable: static const char *fft_name = "Input FFT";
	// Unused variable: static const char *ifft_name = "Output IFFT";
	// Unused variable: static const char *filter_name = "Filter product";

	FFTConfig cfg_fft;
	SpecProductConfig cfg_prod;
	IFFTConfig cfg_ifft;

	cfg_fft.SetAudioSize(size);


	cfg_ifft.SetAudioSize(size);

	if (!InputFFT.Configure(cfg_fft))
		return false;

	if (!Filter.Configure(cfg_prod))
		return false;

	if (!OutputIFFT.Configure(cfg_ifft))
		return false;

	return true;

}

void BigProcessing::ConfigureData(int audio_size)
{
	int spectrum_size = audio_size / 2 + 1;

	ImpulseResponse.SetSize(spectrum_size);
	SpectralInput.SetSize(spectrum_size);
	SpectralOutput.SetSize(spectrum_size);
}


bool BigProcessing::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	int size=0;
	if (mConfig.HasAudioSize())
		size = mConfig.GetAudioSize();

	ConfigureChildren(size);

	ConfigureData(size);

	return true;
}

bool BigProcessing::Do(void)
{
	return false;
}


bool BigProcessing::Do(Audio& in, Audio &out)
{
	InputFFT.Do(in,SpectralInput);

	Filter.Do(SpectralInput,ImpulseResponse,SpectralOutput);

	OutputIFFT.Do(SpectralOutput,out);

	return true;
}

void BigConfiguration::DefaultInit()
{
	AddName();
	AddAudioSize();

	UpdateData();

	SetAudioSize(0);
}


int main()
{
	try {
		BigProcessing system;
		BigConfiguration system_config;

		Audio Input;
		Audio Output;

		Input.SetSize(1024);
		Output.SetSize(1024);
		system_config.SetAudioSize(1024);

		system.Configure(system_config);

		system.Start();

		system.Do(Input,Output);

	} catch(Err &e) {
		e.Print();
	}
	return 0;
}

