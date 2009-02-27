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

#include <CLAM/Enum.hxx>

namespace CLAM
{

class EInterpolatorType : public Enum
{
public:
	EInterpolatorType() : Enum(ValueTable(), SRC_SINC_FASTEST) {}
	EInterpolatorType(tValue v) : Enum(ValueTable(), v) {};
	EInterpolatorType(std::string s) : Enum(ValueTable(), s) {};
	virtual Component* Species() const { return new EInterpolatorType(SRC_SINC_FASTEST); }

	static tEnumValue * ValueTable()
	{
		static tEnumValue sValueTable[] =
		{
			{SRC_SINC_BEST_QUALITY,"SINC_BEST_QUALITY"},
			{SRC_SINC_MEDIUM_QUALITY,"SINC_MEDIUM_QUALITY"},
			{SRC_SINC_FASTEST,"SINC_FASTEST"},
			{SRC_ZERO_ORDER_HOLD,"ZERO_ORDER_HOLD"},
			{SRC_LINEAR,"LINEAR"},
			{0,NULL}
		};
		return sValueTable;
	}
};

class ResamplerByRatioConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (ResamplerByRatioConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, float, Ratio);
	DYN_ATTRIBUTE (1, public, EInterpolatorType, Interpolator);
protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetRatio(1.);
		SetInterpolator(SRC_SINC_FASTEST);
	}
};

/**
* \brief Resampler by ratio.
*
*	Versatile resampling processing only based in the resampling ratio, but with many configuration options for interpolation method.
*	Useful for upsampling and downsampling by factors (x2, etc)
*
*
*	Interpolator options (from SRC documentation: http://www.mega-nerd.com/SRC/api_misc.html):
*
*	SRC_SINC_BEST_QUALITY - This is a bandlimited interpolator derived from the mathematical sinc function and this is the highest quality sinc
*	  based converter, providing a worst case Signal-to-Noise Ratio (SNR) of 97 decibels (dB) at a bandwidth of 97%. All three SRC_SINC_* converters
*	  are based on the techniques of Julius O. Smith although this code was developed independantly.
*	SRC_SINC_MEDIUM_QUALITY - This is another bandlimited interpolator much like the previous one. It has an SNR of 97dB and a bandwidth of 90%.
*	  The speed of the conversion is much faster than the previous one.
*	SRC_SINC_FASTEST - This is the fastest bandlimited interpolator and has an SNR of 97dB and a bandwidth of 80%.
*	SRC_ZERO_ORDER_HOLD - A Zero Order Hold converter (interpolated value is equal to the last value). The quality is poor but the conversion speed
*	  is blindlingly fast.
*	SRC_LINEAR - A linear converter. Again the quality is poor, but the conversion speed is blindingly fast
* 
*/
class ResamplerByRatio : public Processing
{
public:
	typedef ResamplerByRatioConfig Config;
protected:
	AudioInPort _input;
	AudioOutPort _output;

	Config mConfig;

	EInterpolatorType mInterpolatorType;
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

		int error = src_simple ( &resampleData, mConfig.GetInterpolator(), 1);

		std::cout << (error?src_strerror(error):".") << std::flush;
		// Tell the ports this is done
		_input.Consume();
		_output.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ResamplerByRatio_hxx
