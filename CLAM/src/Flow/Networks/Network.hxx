/*
 * Copyright (c) 2008 Fundació Barcelona Media Universitat Pompeu Fabra
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


#ifndef _Network_hxx_
#define _Network_hxx_


//#include "Component.hxx"
//#include "Storage.hxx"
//#include "BaseNetwork.hxx"
#include "FlattenedNetwork.hxx"

namespace CLAM
{
//	typedef FlattenedNetwork Network;

//TODO: this would be derived from BaseNetwork in the next steps of Network refactoring!
class Network : public FlattenedNetwork
{
public:
	// constructor / destructor
	Network() { }
	virtual ~Network() {}
};

}
#endif

