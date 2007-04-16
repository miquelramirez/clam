/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __INSTRUMENT__
#define __INSTRUMENT__

#include "ProcessingComposite.hxx"
#include "ADSR.hxx"
#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"
#include "ControlMapper.hxx"
#include "ControlMultiplier.hxx"

namespace CLAM
{
	class Instrument: public ProcessingComposite
	{
	private:
		AudioOutPort	mOut;
		enum Status {
			eDone = 0,
			eBusy = 1
		} mStatus;

		int                           mId;

	protected:
		InControlTmpl< Instrument >   mStateIn;
		InControlTmpl< Instrument >   mNoteIn;
		InControlTmpl< Instrument >   mVelocityIn;

		OutControl                    mStateOut;
		OutControl                    mNoteOut;
		OutControl                    mVelocityOut;

	public:
		friend class Dispatcher;
		
		void SetId(int id) { mId = id; }

		Instrument():
			mOut("AudioOut",this),	
			mStatus( eDone ),
			mStateIn( "StateIn", this, &Instrument::UpdateState ),
			mNoteIn( "Note", this, &Instrument::UpdateNote ),
			mVelocityIn( "Velocity", this, &Instrument::UpdateVel ),

			mStateOut( "State", this ),
			mNoteOut( "NoteOut", this ),
			mVelocityOut( "VelocityOut", this )
		{
		}
		
 		void LinkStateOutWithInControl(Processing* inProc, unsigned inId)
		{
			GetOutControls().GetByNumber(0).AddLink( inProc->GetInControls().GetByNumber(inId));
//			LinkOutWithInControl( 0, inProc, inId );
		}

		int UpdateState( TControlData value )
		{
			if ( value == 0.0 )
			{
				mStateOut.SendControl( mId ); 
				mStatus = eDone;
			}
			else
				mStatus = eBusy;
			
			return 0;
		}

		int UpdateNote( TControlData value )
		{
			mNoteOut.SendControl( value );

			return 0;
		}
		
		int UpdateVel( TControlData value )
		{
			mVelocityOut.SendControl( value );

			return 0;
		}

	public:
		virtual bool Do(Audio& audio) = 0;
		bool Do(void)
		{
			bool ret = Do(mOut.GetAudio());
			mOut.Produce();
			return ret;
		}
		const char * GetClassName() const {return "Instrument";}
	};
}

#endif

