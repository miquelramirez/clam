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

#ifndef IRSwappingDelay_hxx
#define IRSwappingDelay_hxx

#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Filename.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <map>
#include <dirent.h>
#include <fstream>
#include <iomanip>

namespace CLAM
{
/**
 This processing provides two delay lines from a IR stream
 so that it is compatible with CLAM::XFadeAndSwitch.
 That is, when the IR changes, it keeps it on one of the
 delay lines for two frames. The lines that it is delayed
 is alternated each change starting with the first one.
 Any successive IR change is retained until the 2 frames
 delay of the last change is applied.

Alternating the delayed output:
@code
i:  AAABBBCCC
o1: AAAAABCCC
o2: AAABBBBBC
    AAAAbBBcC
@endcode
The faster change that can be followed, two frames
@code
i:  AAABBCCC
o1: AAAAACCC
o2: AAABBBBC
    AAAAbBcC
@endcode
Deferred change of C because it was too short same output as above:
@code
i:  AAABCCCC
o1: AAAAACCC
o2: AAABBBBC
    AAAAbBcC
@endcode
In this case C is not even outputed
@code
i:  AAABCDDD
o1: AAAAADDD
o2: AAABBBBD
    AAAAbBdD
@endcode
 @ingroup RealTimeConvolution
*/

class IRSwappingDelay : public Processing
{ 
private:
	
	InPort< ImpulseResponse* > _inputIR;
	OutPort< ImpulseResponse* > _line1;
	OutPort< ImpulseResponse* > _line2;
	ImpulseResponse * _current; ///< The current IR selected, not necessarily the incoming one
	ImpulseResponse * _delayed; ///< The old IR during a transition
	bool _delayFirst; ///< Whether we are delaying the first line instead the second one

public:
	const char* GetClassName() const { return "IRSwappingDelay"; }
	IRSwappingDelay(const Config& config = Config()) 
		: _inputIR("Input IR", this)
		, _line1("Current IR", this)
		, _line2("Previous IR", this)
		, _current(0)
		, _delayed(0)
		, _delayFirst(true)
	{
		Configure( config );
	}

	bool ConcreteStart()
	{
		_current = 0;
		_delayed = 0;
		_delayFirst = true;
		return true;
	}

public:
	bool Do()
	{
		ImpulseResponse * incomingIR = _inputIR.GetData();
		OutPort<ImpulseResponse*> & delayed = _delayFirst ? _line1 : _line2;
		OutPort<ImpulseResponse*> & atTime  = _delayFirst ? _line2 : _line1;
		if (not _current)
		{
			// First frame ever, turn it into a regular frame
			_current = incomingIR;
			_delayed = 0;
		}

		if (_delayed)
		{
			// Second frame of a transition that resolves it
			// Changes on incomingIR are ignored
			delayed.GetData() = _delayed;
			atTime.GetData() = _current;
			_delayFirst = not _delayFirst; // switching the delayed line
			_delayed = 0;
		}
		else if (incomingIR != _current)
		{
			// A transition begins
			_delayed = _current;
			_current = incomingIR;
			delayed.GetData() = _delayed;
			atTime.GetData() = _current;
		}
		else
		{
			// Steady state, just take the current
			delayed.GetData() = _current;
			atTime.GetData() = _current;
		}
		_inputIR.Consume();
		_line1.Produce();
		_line2.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // IRSwappingDelay_hxx

