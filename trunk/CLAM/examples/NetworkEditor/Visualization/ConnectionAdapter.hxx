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
#include "Network.hxx"
#include <string>
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
	class ConnectionAdapter : public ModelAdapter
	{
	protected:		
		const CLAM::Network*  mNetworkObserved;
		
	public:
		ConnectionAdapter() : mNetworkObserved(0)
		{
		}
		virtual const char* GetClassName() const
		{
			return "ConnectionAdapter";
		}

		virtual bool Publish()=0;

		virtual std::string GetInName() = 0;
		virtual std::string GetOutName() = 0;
	};


	template<class OUT, class IN>
	class ConnectionAdapterTmpl : public ConnectionAdapter
	{
	private:		
		const OUT* mOutObserved;
		const IN* mInObserved;
	public:
		ConnectionAdapterTmpl();
		const char* GetClassName() const
		{
			return "ConnectionAdapterTmpl";
		}

		std::string GetOutName();
		std::string GetInName();		

		bool ConnectsInElement( IN &);
		bool Publish();
		bool BindTo( const OUT &, const IN &, const CLAM::Network &);
	};





	template<class OUT, class IN>
	ConnectionAdapterTmpl<OUT,IN>::ConnectionAdapterTmpl()
		: mOutObserved(0), mInObserved(0)
	{
	}

	template<class OUT, class IN>
	bool ConnectionAdapterTmpl<OUT,IN>::Publish()
	{
		if ((!mOutObserved)  || (!mInObserved) || (!mNetworkObserved))
			return false;
		
	}
	
	template<class OUT, class IN>
	std::string ConnectionAdapterTmpl<OUT,IN>::GetOutName()
	{
		std::string name("");

		CLAM::Network::ProcessingsMap::const_iterator it;
		for(it=mNetworkObserved->BeginProcessings(); it!=mNetworkObserved->EndProcessings(); it++)
		{
			if( (it->second) == (mOutObserved->GetProcessing()) )
			{
				name += it->first;
				break;
			}
		}

		CLAM_ASSERT( name!="", "ConnectionAdapterTmpl::GetOutName : Processing doesn't exist" );
		
		name += ".";
		name += mOutObserved->GetName();

		return name;

	}
	
	template<class OUT, class IN>
	std::string ConnectionAdapterTmpl<OUT,IN>::GetInName()
	{
		std::string name("");

		CLAM::Network::ProcessingsMap::const_iterator it;
		for(it=mNetworkObserved->BeginProcessings(); it!=mNetworkObserved->EndProcessings(); it++)
		{
			if( (it->second) == (mInObserved->GetProcessing()) )
			{
				name += it->first;
				break;
			}
		}
		CLAM_ASSERT( name!="", "ConnectionAdapterTmpl::GetOutName : Processing doesn't exist" );
		
		name += ".";
		name += mInObserved->GetName();

		return name;
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
