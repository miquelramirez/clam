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

#include "InPortRegistry.hxx"
#include "InPort.hxx"

namespace CLAM
{

InPortBase& InPortRegistry::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Port must be >=0");
	CLAM_ASSERT(index<Size(), "index for Port must be < than Size");
	ConstIterator it;
	int i;
	for (it=mInPorts.begin(), i=0; it!=mInPorts.end(); it++, i++)
	{
		if (i==index) return *(*it);
	}

	CLAM_ASSERT(false, "InPortRegistry::GetByNumber() index out of range");

	return *(InPortBase*)NULL; // just to get rid of warnings
}
	
InPortBase& InPortRegistry::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mInPorts.begin(); it!=mInPorts.end(); it++)
		if(name == (*it)->GetName()) 
			return **it;

	std::string error( "name not found in InPorts collection: " );
	error += "'" +  name + "'" + ". In ports availables: " + AvailableNames();
	CLAM_ASSERT( false, error.c_str() );

	return *(InPortBase*)NULL; // just to get rid of warnings
}

int InPortRegistry::Size() const
{
	return mInPorts.size();
}

InPortRegistry::Iterator  InPortRegistry::Begin()
{
	return mInPorts.begin();
}

InPortRegistry::Iterator InPortRegistry::End()
{
	return mInPorts.end();
}

InPortRegistry::ConstIterator  InPortRegistry::Begin() const
{
	return mInPorts.begin();
}

InPortRegistry::ConstIterator InPortRegistry::End() const
{
	return mInPorts.end();
}


bool InPortRegistry::AreReadyForReading()
{
	Iterator in;
	for ( in=mInPorts.begin(); in!=mInPorts.end(); in++)
		if (!(*in)->CanConsume()) return false;

	return true;
}

void InPortRegistry::Publish( InPortBase * in )
{
	mInPorts.push_back( in );
}
	
std::string InPortRegistry::AvailableNames() const
{
	std::string result;
	ConstIterator it;
	bool first=true;
	for (it=mInPorts.begin(); it!=mInPorts.end(); it++)
	{
		if (!first) 
			result += ", ";
		else 
			first = false;
		InPortBase & port = *(*it);
		result += "'";
		result += port.GetName();
		result += "'";
	}
	return result;
}
} // namespace CLAM

