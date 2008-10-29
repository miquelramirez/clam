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

#include "Dispatcher.hxx"

using namespace CLAM;

bool Dispatcher::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);
	int i,j,k;

	mInstruments = mConfig.GetInstruments();
	mNInValues = mConfig.GetNInValues();
	mMInstruments = mInstruments.Size();

	for( i = 0; i < mMInstruments; i++ )
	{
		mInstruments[ i ]->SetId(i);
		mInstruments[ i ]->LinkStateOutWithInControl( this, 0 ) ;

		InstrStatus status = {-1,-1,i};
		mInstrStatusList.push_back(status);
	}

	k = 0;

	for ( i = 0; i < mMInstruments; i++ )
	{
		for ( j = 0; j < mNInValues;j++)
		{
			mValuesOut.AddElem( new FloatOutControl("",this ) );
			GetOutControl(k).AddLink(mInstruments[i]->GetInControl(j+1));
			//LinkOutWithInControl( k , mInstruments[ i ], j+1);
			k++;
		}
	}

	return true;
}

int Dispatcher::UpdateState( TControlData availableInstr )
{
	std::list<InstrStatus>::iterator it;
	for (it=mInstrStatusList.begin();it!=mInstrStatusList.end();it++)
	{
		if ((*it).mId == availableInstr)
		{
			mInstrStatusList.erase(it);
			InstrStatus status = { -1,-1, int(availableInstr) };
			mInstrStatusList.push_front(status);
			return 0;
		}
	}
	return 0;
}

void Dispatcher::Dispatch(void)
{
	if( mVelocity == 0.0 ) 
	{
		std::list<InstrStatus>::iterator it;

		for (it=mInstrStatusList.begin();it!=mInstrStatusList.end();it++)
		{
			if ( ( (*it).mNote == mNote ) && ( (*it).mVelocity ) )
			{
				InstrStatus status = (*it);
				(*it).mVelocity = 0;
				mValuesOut[ (*it).mId * mNInValues + 1]->SendControl( mVelocity );
				return;
			}
		}
	}
	else
	{
		InstrStatus status = mInstrStatusList.front();
		mInstrStatusList.pop_front();
		status.mNote = int(mNote);
		status.mVelocity = int(mVelocity);
		mInstrStatusList.push_back(status);

		mValuesOut[ status.mId * mNInValues + 1 ]->SendControl( mVelocity );
		mValuesOut[ status.mId * mNInValues ]->SendControl( mNote );
	}
}


