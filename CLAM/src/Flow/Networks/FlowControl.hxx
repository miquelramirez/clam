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


#ifndef _FlowControl_hxx_
#define _FlowControl_hxx_


#include <list>

namespace CLAM
{

class Network;
class NodeBase;
class Processing;

class FlowControl
{
	bool mNetworkChanged;

public:
	FlowControl( int frameSize = 512 );
	virtual ~FlowControl(){}
	virtual void AttachToNetwork( Network* );
	
	//methods relative to state of FlowControl & Network
	virtual void ProcessingAddedToNetwork( Processing& added );
	virtual void ProcessingRemovedFromNetwork( Processing& removed );
	void ProcessingConfigured( Processing & configured );

	void ConfigureNode( NodeBase& toConfigure ) const;
	
	// to implement in each type of flowcontrol
	virtual void DoProcessings() = 0;

	void NetworkTopologyChanged() { mNetworkChanged=true;}

protected:
	bool HasNetworkTopologyChanged() const { return mNetworkChanged; }
	void ConfigurePorts(Processing &toConfigure) const;
	
	int mFrameSize;
	Network * mNetwork;

};

}

#endif
