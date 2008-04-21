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

#ifndef AudioBuffer2Stream_hxx
#define AudioBuffer2Stream_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>

namespace CLAM
{

class AudioBuffer2Stream : public Processing
{
	// TODO: This configuration is over crowed
	typedef AudioWindowingConfig Config;
	InPort<Audio> mIn;
	AudioOutPort mOut;
	unsigned mHopSize;
	unsigned mWindowSize;
	Config mConfig;
public:
	const char* GetClassName() const { return "AudioBuffer2Stream"; }
	AudioBuffer2Stream(const Config& config = Config()) 
		: mIn("Audio buffer", this)
		, mOut("Audio stream", this) 
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & c)
	{
		CopyAsConcreteConfig(mConfig, c);
		mHopSize = mConfig.GetHopSize();
		mWindowSize = mConfig.GetFFTSize();
		mOut.SetSize( mWindowSize );
		mOut.SetHop( mHopSize );
		return true;
	}
	const ProcessingConfig & GetConfig() const { return mConfig; }
 
	bool Do()
	{
		const Audio& in = mIn.GetData();
		CLAM_ASSERT(mWindowSize==in.GetSize(),
			"AudioBuffer2Stream: Input does not provide the configured window size"); 
		Audio& out = mOut.GetAudio();
		const TData* inpointer = in.GetBuffer().GetPtr();
		TData* outpointer = out.GetBuffer().GetPtr();
		// Zero fill the new incomming hopSize
		std::fill(outpointer+mWindowSize-mHopSize, outpointer+mWindowSize, 0.0);
		// Add the input on the full window
		std::transform(inpointer, inpointer+mWindowSize, outpointer, outpointer, std::plus<TData>());
		
		// Tell the ports this is done
		mIn.Consume();
		mOut.Produce();
		return true;
	}


};

} // namespace CLAM
#endif // AudioBuffer2Stream_hxx
