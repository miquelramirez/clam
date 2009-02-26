/*
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

#ifndef ResamplerByRatio_hxx
#define ResamplerByRatio_hxx

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <samplerate.h>

namespace CLAM
{

class ResamplerByRatioConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (ResamplerByRatioConfig, 1, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, float, Ratio);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetRatio(1.);
	}
};

/**
* \brief Resampler by ratio.
*
* Versatile resampling processing only based in the resampling ratio
* Useful for upsampling and downsampling by factors (x2, etc)
*/
class ResamplerByRatio : public Processing
{
public:
	typedef ResamplerByRatioConfig Config;
private:
	AudioInPort _input;
	AudioOutPort _output;
	ResamplerByRatioConfig mConfig;
public:
	const char* GetClassName() const { return "ResamplerByRatio"; }
	ResamplerByRatio(const ResamplerByRatioConfig & config = ResamplerByRatioConfig()) 
		: _input("Input", this)
		, _output("Output", this) 
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & c)
	{
		CopyAsConcreteConfig(mConfig, c);

		_output.SetSize( mConfig.GetRatio()*float(_input.GetSize()) );
		_output.SetHop( mConfig.GetRatio()*float(_input.GetSize()) );
		_output.SetSampleRate( mConfig.GetRatio()*float(_input.GetSampleRate()) );
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
		resampleData.src_ratio = mConfig.GetRatio();

		int error = src_simple ( &resampleData, SRC_SINC_FASTEST, 1);

		std::cout << (error?src_strerror(error):".") << std::flush;
		// Tell the ports this is done
		_input.Consume();
		_output.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ResamplerByRatio_hxx
