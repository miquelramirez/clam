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

#ifndef _CONNECTIONADAPTER_HXX_
#define _CONNECTIONADAPTER_HXX_

#include "ModelAdapter.hxx"
#include "ConnectionModel.hxx"
#include "Network.hxx"

#include <list>

namespace CLAM
{
	class OutPort;
	class InPort;
	class InControl;
	class OutControl;
	class Network;
}

namespace CLAMVM
{
	class ConnectionAdapter : public ModelAdapter, public ConnectionModel
	{
	protected:		
//		const CLAM::OutPort* mOutObserved;
//		const CLAM::InPort* mInObserved;
		const CLAM::Network*  mNetworkObserved;
		
	public:
		ConnectionAdapter();
		virtual ~ConnectionAdapter();
		virtual const char* GetClassName() const
		{
			return "ConnectionAdapter";
		}

//		virtual bool ConnectsInPort( CLAM::InPort &);
		virtual bool Publish()=0;
//		virtual bool BindTo( const CLAM::OutPort&, const CLAM::InPort&, const CLAM::Network &);
		
	};


	template<class OUT, class IN>
	class ConnectionAdapterTmpl : public ConnectionAdapter
	{
	private:		
		const OUT* mOutObserved;
		const IN* mInObserved;
	public:
		ConnectionAdapterTmpl();
		virtual ~ConnectionAdapterTmpl();

		virtual const char* GetClassName() const
		{
			return "ConnectionAdapterTmpl";
		}

		virtual bool ConnectsInElement( IN &);
		virtual bool Publish();
		virtual bool BindTo( const OUT &, const IN &, const CLAM::Network &);
	};





	template<class OUT, class IN>
	ConnectionAdapterTmpl<OUT,IN>::ConnectionAdapterTmpl()
		: mOutObserved(0), mInObserved(0)
	{
	}

	template<class OUT, class IN>
	ConnectionAdapterTmpl<OUT,IN>::~ConnectionAdapterTmpl()
	{
	}
	
	template<class OUT, class IN>
	bool ConnectionAdapterTmpl<OUT,IN>::Publish()
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


	template<class OUT, class IN>
	bool ConnectionAdapterTmpl<OUT,IN>::BindTo( const OUT & out, 
						    const IN & in, 
						    const CLAM::Network & net )
	{
		mOutObserved = &out;
		mInObserved = &in;
		mNetworkObserved = &net;

		return true;
	}

	template<class OUT, class IN>
	bool ConnectionAdapterTmpl<OUT,IN>::ConnectsInElement( IN & in)
	{
		return (&in == mInObserved);
	}
} // namespace CLAMVM

#endif //_CONNECTIONADAPTER_HXX_
