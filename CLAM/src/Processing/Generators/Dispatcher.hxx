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

#ifndef __DISPATCHER__
#define __DISPATCHER__

#include "Array.hxx"
#include "Instrument.hxx"

#include <algorithm>

namespace CLAM
{

	class DispatcherConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (DispatcherConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, Array< Instrument* >, Instruments);
		DYN_ATTRIBUTE (1, public, int, NInValues);

	protected:
		void DefaultInit(void)
		{
			AddInstruments();
			AddNInValues();
			UpdateData();
		}
	};

	class Dispatcher:public Processing
	{
	private:
		struct InstrStatus
		{
		public:
				int  mNote;
				int	 mVelocity;
				int  mId;
		};

		DispatcherConfig                      mConfig;
		Array< Instrument* >                  mInstruments;
		Array< OutControl* >                  mValuesOut;
		InControlTmpl< Dispatcher >           mStateIn;
		InControlTmpl< Dispatcher >           mNoteIn;
		InControlTmpl< Dispatcher >           mVelocityIn;
		int                                   mNInValues;
		int                                   mMInstruments;
		TControlData                          mVelocity;
		TControlData                          mNote;
		std::list< InstrStatus >			  mInstrStatusList;
	
	protected:

		int UpdateState( TControlData availableInstr );

		int UpdateVel( TControlData value )
		{
//			printf("UpdateVel = %f\n",value);
			mVelocity = value;

			return 0;
		}

		int UpdateNote( TControlData value )
		{
//			printf("UpdateNote = %f\n",value);
			mNote = value;
			Dispatch();

			return 0;
		}

		void Dispatch(void);

	public:

		Dispatcher():
		  mStateIn("",this,&Dispatcher::UpdateState),
		  mNoteIn( "Note", this, &Dispatcher::UpdateNote ),
		  mVelocityIn( "Velocity", this, &Dispatcher::UpdateVel ),
		  mVelocity( 0 ),
		  mNote( 0 )
		{
			DispatcherConfig cfg;

			Configure( cfg );
		}

		Dispatcher( const DispatcherConfig& c):
		  mStateIn("",this,&Dispatcher::UpdateState),
		  mNoteIn( "Note", this, &Dispatcher::UpdateNote ),
		  mVelocityIn( "Velocity", this, &Dispatcher::UpdateVel ),
		  mVelocity( 0 ),
		  mNote( 0 )
		{
			  Configure( c );
		}

		~Dispatcher(){}

		const char * GetClassName() const {return "Dispatcher";}

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool ConcreteConfigure( const ProcessingConfig& c );

		bool Do(void) { return true; }

	};
}

#endif

