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

	protected:
		friend class Dispatcher;
		
		void SetId(int id) { mId = id; }

		Instrument():
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
			GetOutControls().GetByNumber(0).AddLink(&inProc->GetInControls().GetByNumber(inId));
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
		const char * GetClassName() const {return "Instrument";}
	};
}

#endif
