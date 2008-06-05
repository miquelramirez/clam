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

#ifndef XFadeAndSwitch_hxx
#define XFadeAndSwitch_hxx

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include "ComplexSpectrum.hxx"
#include <vector>

namespace CLAM
{
/**
 * Enable the convolution with an impulse response that changes 
 * along the time, avoiding clicks on the change by properly crossfading
 * when an impulse response is detected.
 *
 * It address two problems, the one is the progressive transition
 * over signals, and the other is the mix of signals on some frames
 * due to the overlap and add.
 * This processing requires the setting of two convolution lines
 * which are fed as indicated bellow to discard such crapped frames,
 * do the crossfading when the signal is stable and switch to the new
 * signal afterwards.
 *
 * To understand the process see the following sample sequence:
 * Being A,B,C,D different signals
 * Being A0 the 0th audio frame of the A signal coming from the iFFT, 
 * having double the framesize.
 * Being a0 and a0' the two semibuffers of A0 having the frame size
 * which are added a0 at frame 0 and a0' at frame 1 during the overlap&add.
 * @code
 * Changes:                 B                                   C        D
 * ifft  1:   A1   |   A2   |   A3   |   A4   |   B5   |   B6   |   C7   |   C8   |   C9   |   C10   |
 * o&add 1: a0'+a1 | a1'+a2 | a2'+a3 | a3'+a4 | a4'+b5 | b5'+b6 | a6'+c7 | c7'+c8 | c8'+c9 | c9'+c10 |
 * ifft  2:   A1   |   A2   |   B3   |   B4   |   B5   |   B6   |   B7   |   B8   |   D9   |   D10   |
 * o&add 2: a0'+a1 | a1'+a2 | a2'+b3 | b3'+b4 | b4'+b5 | b5'+b6 | b6'+b7 | b7'+b8 | b8'+d9 | d9'+d10 |
 * xf&swch:    1        1        1       1X2       2       2         2       2X1       1       1X2
 * result:     A        A        A       AXB       B       B         B       BXC       C       CXD
 * @endcode
 *
 * This impose several conditions to the feeder:
 * - On a signal change one of the lines should keep the old signal during 2 frames
 * - The first line to keep the old signal is 1 and then they alternate.
 * - Any other signal change during such 2 frames should be ignored (see D in the example)
 */
class XFadeAndSwitch : public Processing
{
public:
	typedef std::vector<ComplexSpectrum>* ImpulseResponse;
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);

	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(512);
	    };
	};

	AudioInPort _in1;
	InPort<ImpulseResponse> _impulseResponse1;
	AudioInPort _in2;
	InPort<ImpulseResponse> _impulseResponse2;
	AudioOutPort _out;
	bool _inTransition;
	bool _currentIs1;
public:
	const char* GetClassName() const { return "XFadeAndSwitch"; }
	XFadeAndSwitch(const Config& config = Config()) 
		: _in1("FadingIn", this)
		, _impulseResponse1("ImpulseResponse1", this)
		, _in2("FadingOut", this)
		, _impulseResponse2("ImpulseResponse2", this)
		, _out("CrossFadedBuffer", this) 
		, _inTransition(false)
		, _currentIs1(true)
	{
		Configure( config );
		_in1.SetHop(512);
		_in1.SetSize(512);
		_in2.SetHop(512);
		_out.SetSize(512);
		_out.SetHop(512);
		_in2.SetSize(512);
		_inTransition=false;
		_currentIs1=true;
	}
 
	bool Do()
	{
		const ImpulseResponse & ir1 = _impulseResponse1.GetData();
		const ImpulseResponse & ir2 = _impulseResponse2.GetData();
		const TData * inpointer1 = & _in1.GetData();
		const TData * inpointer2 = & _in2.GetData();
		TData* outpointer = & _out.GetData();
		const unsigned size = _in1.GetSize();
		if (_inTransition)
		{
			// Crossfade and switch
			TData step = 1./size;
			TData level = 0;
			const TData * current = _currentIs1?inpointer1:inpointer2;
			const TData * newOne = _currentIs1?inpointer2:inpointer1;
			for (unsigned i = 0; i<size; i++, level+=step)
			{
				outpointer[i]=level*newOne[i];
				outpointer[i]+=(1-level)*current[i];
			}
			_currentIs1 = not _currentIs1;
		}
		else
		{
			// Just pass the current one
			memcpy(outpointer,(_currentIs1?inpointer1:inpointer2), size*sizeof(TData));
			// Mark whether a transition is detected to xfade the next frame
			// We are not xfading this frame as overlap and add messed it
			_inTransition = (ir1 != ir2);
		}

		// Tell the ports this is done
		_in1.Consume();
		_in2.Consume();
		_impulseResponse1.Consume();
		_impulseResponse2.Consume();
		_out.Produce();
		return true;
	}


};

} // namespace CLAM
#endif // XFadeAndSwitch_hxx
