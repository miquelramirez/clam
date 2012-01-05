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

#ifndef _Fund2MIDI_
#define _Fund2MIDI_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "OutControl.hxx"
#include "OutControl.hxx"
#include "Fundamental.hxx"
#include "MIDIMessage.hxx"

namespace CLAM
{

	/** 	\brief Converts fundamental to a MIDI note value
	*
	*	Things to know:
	*	C4 or "middle c" has a MIDI value of 60, then A4 (440Hz) has a MIDI value of 69
	*	Taking that in account you can arrive to:
	*	\f$ fund_{midinote} = 69+log_{2^{(\frac{1}{12})}}(\frac{fundfrec}{440Hz}) \f$ and because
	*	\f$ log_{2^{\frac{1}{12}}}(a) = log_{e}(a)*17.31234 \f$ then:
	*	\f$ fund_{midinote} = 69+log_{e}(\frac{fundfrec}{440Hz})*17.31234 \f$.
	*/
	class Fund2MIDI : public Processing
	{
		InPort< Fundamental > mInFund;
		FloatOutControl mFreqControlOut;
		OutControl< MIDI::Message > mOutputMIDIMessage;

		Config mConfig;

		TSize _previousNote; ///< MIDI note previously sent
	public:

		const char *GetClassName() const { return "Fund2MIDI"; }

		Fund2MIDI( const ProcessingConfig & cfg = Config()  ) 
			: mInFund("In Fundamental", this)
			, mFreqControlOut("Frequency", this)
			, mOutputMIDIMessage("MIDI Message Out", this)
			, _previousNote(0)
		{ 
			Configure( cfg );
		}

		bool Do()
		{
			bool result = Do( mInFund.GetData() );
			mInFund.Consume();
			return result;
		}

		bool Do( const Fundamental& inFund );
};

}

#endif

