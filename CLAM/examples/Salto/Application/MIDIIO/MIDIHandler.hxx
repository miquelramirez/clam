#ifndef __MIDIHANDLER__
#define __MIDIHANDLER__

#include "ProcessingComposite.hxx"
#include "Parameters.hxx"
#include "Controller.hxx"
#include <iostream>

namespace CLAM
{
	class MIDIHandlerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDIHandlerConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, double, PitchModRange);
	protected:
		void DefaultInit(void);
	};

	class MIDIHandler: public ProcessingComposite
	{
	public:	
		MIDIHandler();
		MIDIHandler( const MIDIHandlerConfig& cfg );

		virtual ~MIDIHandler();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		void SetParams(Parameters* params) { mParams = params; }

		void LinkOutControl( unsigned in, Processing* processing, unsigned out)
		{
			 mControl.OutControls[ out ].AddLink( processing->GetInControl( in ) );
		}

	private:
		MIDIHandlerConfig							mConfig;

		InControlTmpl< MIDIHandler >				mVelocityIn;
		InControlTmpl< MIDIHandler >				mNoteIn;
		InControlTmpl< MIDIHandler >				mPitchBendIn;
		InControlTmpl< MIDIHandler >				mAirSpeedIn;

		TControlData mVelocity;
		
		Parameters* mParams;
		Parameters& Params(void) { return *mParams; }

		typedef enum{
			eIdle = 0,
			eAttack =1,
			eTransition = 2,
			eRelease = 4
		} eState;

		typedef enum{
			eStateControl = 0,
			eNUMControls 
		} eControls;

		Controller	mControl;
	public:
		bool Do(void){ return mControl.Do(); }

		bool DoNote(void)
		{ 
			if( Params().GetPlayNoteOn() ) // NoteOn
			{
				mControl.EnqueueControl( eStateControl ,(TControlData( eAttack )));
				Params().SetPlayNoteOn( false );
			}
			else if( Params().GetPlayNoteOff() ) // NoteOff
			{
				Params().SetPlayNote( false );
				Params().SetPlayNoteOff( false );
				mControl.EnqueueControl( eStateControl ,(TControlData( eRelease )));
			}
			return mControl.Do();
		}

	protected:
		void AdoptChildren();
		bool ConfigureChildren();
		void InitializeControlValues();
		bool ConcreteConfigure( const ProcessingConfig& c );
		bool ConcreteStart();
		bool ConcreteStop();


		int UpdateVelocity( TControlData velocity )
		{
			mVelocity = velocity;

			return 0;
		}

		int UpdateNote ( TControlData note );

		int UpdatePitchBend( TControlData value );

		int UpdateAirSpeed( TControlData airSpeed );

	};
}

#endif
