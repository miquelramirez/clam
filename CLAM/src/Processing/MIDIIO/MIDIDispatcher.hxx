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
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"

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
		void DefaultInit(void);
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
		InControlTmpl< MIDIDispatcher >           mStateIn;
		InControlTmpl< MIDIDispatcher >           mNoteIn;
		InControlTmpl< MIDIDispatcher >           mVelocityIn;
		int                                   mNInControls;
		int                                   mNVoices;
		TControlData                          mVelocity;
		TControlData                          mNote;
		std::list< VoiceStatus >			  mVoiceStatusList;
	
	protected:

		int UpdateState( TControlData availableInstr );


		int UpdateVel( TControlData value )
		{
			mVelocity = value;

			return 0;
		}


		int UpdateNote( TControlData value )
		{
			mNote = value;
			Dispatch();

			return 0;
		}

		void Dispatch(void);

	public:

		MIDIDispatcher();
		MIDIDispatcher( const MIDIDispatcherConfig& cfg );

		bool ModifiesPortsAndControlsAtConfiguration() { return true; }

		virtual ~MIDIDispatcher(){  RemoveControls(); }

		const char * GetClassName() const {return "MIDIDispatcher";}

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool ConcreteConfigure( const ProcessingConfig& c );

		bool Do() { return true; }

	};
}

#endif

