/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __MIDIDISPATCHER__
#define __MIDIDISPATCHER__

#include "Array.hxx"
#include "Instrument.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

#include <algorithm>

namespace CLAM
{

	class MIDIDispatcherConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDIDispatcherConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE ( 0, public, int, NumberOfInControls );
		DYN_ATTRIBUTE ( 1, public, int, NumberOfVoices );

	protected:
		void DefaultInit(void)
		{
			AddNumberOfVoices();
			AddNumberOfInControls();
			UpdateData();
			SetNumberOfVoices( 2 );
			SetNumberOfInControls( 2 );
		}
	};

	class MIDIDispatcher : public Processing
	{

		std::vector< InControl* > mInputControls;
		std::vector< OutControl* > mOutputControls;

		void CreateControls();
		void RemoveControls();

	private:
		struct VoiceStatus
		{
		public:
				int  mNote;
				int	 mVelocity;
				int  mId;
		};

		MIDIDispatcherConfig                      mConfig;
//		Array< Instrument* >                  mInstruments;
//		Array< OutControl* >                  mValuesOut;
		InControlTmpl< MIDIDispatcher >           mStateIn;
		InControlTmpl< MIDIDispatcher >           mNoteIn;
		InControlTmpl< MIDIDispatcher >           mVelocityIn;
		int                                   mNInControls;
		int                                   mNVoices;
		TControlData                          mVelocity;
		TControlData                          mNote;
		std::list< VoiceStatus >			  mVoiceStatusList;
	
	protected:

		int UpdateState( TControlData availableInstr )
		{
			std::cout << "MD - State updated: " << availableInstr << std::endl;
			std::list<VoiceStatus>::iterator it;

			for (it=mVoiceStatusList.begin();it!=mVoiceStatusList.end();it++)
			{
				if ((*it).mId == availableInstr)
				{
					mVoiceStatusList.erase(it);
					VoiceStatus status = { -1,-1, int(availableInstr) };
					mVoiceStatusList.push_front(status);
					return 0;
				}
			}
			return 0;
		}


		int UpdateVel( TControlData value )
		{
			std::cout << "MD - Velocity updated: " << value << std::endl;
			mVelocity = value;

			return 0;
		}


		int UpdateNote( TControlData value )
		{
			std::cout << "MD - Note updated: " << value << std::endl;
			mNote = value;
			Dispatch();

			return 0;
		}

		void Dispatch(void);

	public:

		MIDIDispatcher()
			:
		  mStateIn("StateIn",this,&MIDIDispatcher::UpdateState),
		  mNoteIn( "Note", this, &MIDIDispatcher::UpdateNote ),
		  mVelocityIn( "Velocity", this, &MIDIDispatcher::UpdateVel ),
		  mVelocity( 0 ),
		  mNote( 0 )
		{
			Configure( mConfig );
		}

		MIDIDispatcher( const MIDIDispatcherConfig& cfg ) 
			:
		  mStateIn("",this,&MIDIDispatcher::UpdateState),
		  mNoteIn( "Note", this, &MIDIDispatcher::UpdateNote ),
		  mVelocityIn( "Velocity", this, &MIDIDispatcher::UpdateVel ),
		  mVelocity( 0 ),
		  mNote( 0 )
		{
			  Configure( cfg );
		}

		bool ModifiesPortsAndControlsAtConfiguration() { return true; }

		virtual ~MIDIDispatcher(){  RemoveControls(); }

		const char * GetClassName() const {return "MIDIDispatcher";}

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool ConcreteConfigure( const ProcessingConfig& c );

		bool Do() { return true; }

	};
}

#endif
