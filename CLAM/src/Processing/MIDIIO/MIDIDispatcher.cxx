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

#include "ProcessingFactory.hxx"
#include "MIDIDispatcher.hxx"


namespace CLAM 
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, MIDIDispatcher> regMIDIDispatcher("MIDIDispatcher");
}

void MIDIDispatcherConfig::DefaultInit()
{
	AddNumberOfVoices();
	AddNumberOfInControls();
	UpdateData();
	SetNumberOfVoices( 2 );
	SetNumberOfInControls( 2 );
}


MIDIDispatcher::MIDIDispatcher()
	:   mStateIn("StateIn",this,&MIDIDispatcher::UpdateState),
		  mNoteIn( "Note", this, &MIDIDispatcher::UpdateNote ),
		  mVelocityIn( "Velocity", this, &MIDIDispatcher::UpdateVel ),
		  mVelocity( 0 ),
		  mNote( 0 )
{
	Configure( mConfig );
}

MIDIDispatcher::MIDIDispatcher( const MIDIDispatcherConfig& cfg )
	:  mStateIn("",this,&MIDIDispatcher::UpdateState),
  		mNoteIn( "Note", this, &MIDIDispatcher::UpdateNote ),
		mVelocityIn( "Velocity", this, &MIDIDispatcher::UpdateVel ),
		mVelocity( 0 ),
		mNote( 0 )
{
	Configure( cfg );
}
	
	

bool MIDIDispatcher::ConcreteConfigure( const ProcessingConfig& cfg )
{

	CopyAsConcreteConfig(mConfig, cfg);
	RemoveControls();
	CreateControls();

	return true;
}


int MIDIDispatcher::UpdateState( TControlData availableInstr )
{
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
	


void MIDIDispatcher::Dispatch(void)
{
	if( mVelocity == 0.0 ) 
	{
		std::list<VoiceStatus>::iterator it;

		for (it=mVoiceStatusList.begin();it!=mVoiceStatusList.end();it++)
		{
			if ( ( (*it).mNote == mNote ) && ( (*it).mVelocity ) )
			{
				VoiceStatus status = (*it);
				(*it).mVelocity = 0;
				//mValuesOut[ (*it).mId * mNInValues + 1]->SendControl( mVelocity );
				mOutputControls[ (*it).mId * + 1]->SendControl( mVelocity );
				return;
			}
		}
	}
	else
	{
		VoiceStatus status = mVoiceStatusList.front();
		mVoiceStatusList.pop_front();
		status.mNote = int(mNote);
		status.mVelocity = int(mVelocity);
		mVoiceStatusList.push_back(status);

		mOutputControls[ status.mId * mConfig.GetNumberOfInControls() + 1 ]->SendControl( mVelocity );
		mOutputControls[ status.mId * mConfig.GetNumberOfInControls() ]->SendControl( mNote );
	}
}

void MIDIDispatcher::CreateControls()
{

	for (int i=0;i<mConfig.GetNumberOfInControls(); i++)
	{
		std::stringstream number("");
		number << i;
		InControl* inControl = new InControl( "InControl " + number.str(), this );
		mInputControls.push_back( inControl );
	}

    for( int i=0; i<mConfig.GetNumberOfInControls(); i++ )
		mInputControls[i]->DoControl(0.);						// set initial value for each InControl
		
	for(int i = 0; i < mConfig.GetNumberOfVoices(); i++ )
	{
//		mInstruments[ i ]->SetId(i);
//		mInstruments[ i ]->LinkStateOutWithInControl( this, 0 ) ;
//
		VoiceStatus status = { -1, -1, i };
		mVoiceStatusList.push_back(status);
	}

	int k = 0;
	for (int i = 0; i < mConfig.GetNumberOfVoices(); i++ )
	{
		for ( int j=0; j < mConfig.GetNumberOfInControls();j++)
		{
			std::stringstream number("");
			number << i << j;
			mOutputControls.push_back( new OutControl("a" + number.str(),this ) );
//			GetOutControls().GetByNumber(k).AddLink( mInstruments[i]->GetImConfig.GetNumberOfInControls().GetByNumber(j+1));
//			LinkOutWithInControl( k , mInstruments[ i ], j+1);
			k++;
		}
	}
}

void MIDIDispatcher::RemoveControls()
{
	std::vector< InControl* >::iterator itInControl;
	
	for( itInControl=mInputControls.begin(); itInControl!=mInputControls.end(); itInControl++)
		delete *itInControl;

	mInputControls.clear();
}


} // namespace CLAM

