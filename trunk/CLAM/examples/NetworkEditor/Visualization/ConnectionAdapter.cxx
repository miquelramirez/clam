/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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


#include "ConnectionAdapter.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"


namespace CLAMVM
{

ConnectionAdapter::ConnectionAdapter()
//	: mOutObserved(0), mInObserved(0)
	: mNetworkObserved(0)
{
}

ConnectionAdapter::~ConnectionAdapter()
{
}
/*
bool ConnectionAdapter::Publish()
{
	if ((!mOutObserved)  || (!mInObserved) || (!mNetworkObserved))
		return false;
	
	std::string outName("");
	std::string inName("");

	CLAM::Network::ProcessingsMap::const_iterator it;
	for(it=mNetworkObserved->BeginProcessings(); it!=mNetworkObserved->EndProcessings(); it++)
	{
		if( (it->second) == (mOutObserved->GetProcessing()) )
		{
			outName += it->first;
		}

		else if( (it->second) == (mInObserved->GetProcessing()) )
		{
			inName += it->first;
		}
	}
			
	outName += ".";
	outName += mOutObserved->GetName();


	inName += ".";
	inName += mInObserved->GetName();
	
	AcquireNames.Emit( outName, inName );
}
*/
/*
bool ConnectionAdapter::BindTo( const CLAM::OutPort& out, 
				const CLAM::InPort& in, 
				const CLAM::Network & net )
{
	mOutObserved = dynamic_cast< const CLAM::OutPort* > (&out);
	mInObserved = dynamic_cast< const CLAM::InPort* > (&in);
	mNetworkObserved = dynamic_cast< const CLAM::Network *> (&net);
	
	if ((!mOutObserved)  || (!mInObserved) || (!mNetworkObserved))
		return false;
	return true;
}
*/
/*
bool ConnectionAdapter::ConnectsInPort( CLAM::InPort & in)
{
	return (&in == mInObserved);
}
*/



} // namespace CLAMVM
