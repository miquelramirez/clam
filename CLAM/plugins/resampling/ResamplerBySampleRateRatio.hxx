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

#ifndef ResamplerBySampleRateRatio_hxx
#define ResamplerBySampleRateRatio_hxx

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <samplerate.h>

#include <CLAM/CLAM_Math.hxx>

namespace CLAM
{

class ResamplerBySampleRateRatioConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (ResamplerBySampleRateRatioConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, unsigned, InputSampleRate);
	DYN_ATTRIBUTE (1, public, unsigned, OutputSampleRate);
protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetInputSampleRate(44100);
		SetOutputSampleRate(48000);
	}
};

/**
* \brief Resampler by SampleRate ratio.
*
* Typical use: 44100Hz to 48000Hz SampleRate converter
*/
class ResamplerBySampleRateRatio : public Processing
{
public:
	typedef ResamplerBySampleRateRatioConfig Config;
private:
	AudioInPort _input;
	AudioOutPort _output;
	ResamplerBySampleRateRatioConfig mConfig;
public:
	const char* GetClassName() const { return "ResamplerBySampleRateRatio"; }
	ResamplerBySampleRateRatio(const ResamplerBySampleRateRatioConfig & config = ResamplerBySampleRateRatioConfig()) 
		: _input("Input", this)
		, _output("Output", this) 
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & c)
	{
		CopyAsConcreteConfig(mConfig, c);

		int outputSize = round( float(_input.GetSize())*mConfig.GetOutputSampleRate()/mConfig.GetInputSampleRate() );

		_output.SetSize( outputSize );
		_output.SetHop( outputSize );
		_output.SetSampleRate( mConfig.GetOutputSampleRate() );
		return true;
	}
	const ProcessingConfig &GetConfig() const { return mConfig;}
	bool Do()
	{
		const Audio & input = _input.GetAudio();
		Audio & output = _output.GetAudio();
		
		SRC_DATA resampleData;
		resampleData.data_in = input.GetBuffer().GetPtr();
		resampleData.data_out = output.GetBuffer().GetPtr();
		resampleData.input_frames = input.GetSize();
		resampleData.output_frames = output.GetSize();
		resampleData.src_ratio = float(output.GetSampleRate())/float(input.GetSampleRate());
// 		int error = src_simple ( &resampleData, SRC_SINC_BEST_QUALITY, 1);
		int error = src_simple ( &resampleData, SRC_SINC_FASTEST, 1);

		std::cout << (error?src_strerror(error):".") << std::flush;
		// Tell the ports this is done
		_input.Consume();
		_output.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ResamplerBySampleRateRatio_hxx
