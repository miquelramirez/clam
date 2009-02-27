/*
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

#include "ResamplerByRatio.hxx"

namespace CLAM
{

class ResamplerBySampleRateRatioConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (ResamplerBySampleRateRatioConfig, 3, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, unsigned, InputSampleRate);
	DYN_ATTRIBUTE (1, public, unsigned, OutputSampleRate);
	DYN_ATTRIBUTE (2, public, EInterpolatorType, Interpolator);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetInputSampleRate(44100);
		SetOutputSampleRate(48000);
		SetInterpolator(SRC_SINC_FASTEST);
	}
};

/**
* \brief Resampler by SampleRate ratio.
*
* Typical use: 44100Hz to 48000Hz SampleRate converter
*/
class ResamplerBySampleRateRatio : public ResamplerByRatio
{
public:
	typedef ResamplerBySampleRateRatioConfig Config;
private:
	Config mConfig;
public:
	const char* GetClassName() const { return "ResamplerBySampleRateRatio"; }

	ResamplerBySampleRateRatio(const ResamplerBySampleRateRatioConfig & config = ResamplerBySampleRateRatioConfig()) 
		: ResamplerByRatio()
	{
	}
	bool ConcreteConfigure(const ProcessingConfig & c)
	{
		CopyAsConcreteConfig(mConfig, c);

		ResamplerByRatio::mConfig.SetRatio( float(mConfig.GetOutputSampleRate())/float(mConfig.GetInputSampleRate()) );

		int outputSize = round( float(_input.GetSize())*mConfig.GetOutputSampleRate()/mConfig.GetInputSampleRate() );

		_output.SetSize( outputSize );
		_output.SetHop( outputSize );
		_output.SetSampleRate( mConfig.GetOutputSampleRate() );

		return true;
	}
	const ProcessingConfig &GetConfig() const { return mConfig;}
};

} // namespace CLAM

#endif // ResamplerBySampleRateRatio_hxx
