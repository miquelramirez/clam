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

#include "OutPortRegistry.hxx"
#include "OutPort.hxx"

namespace CLAM
{

OutPortBase & OutPortRegistry::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Port must be >=0");
	CLAM_ASSERT(index<Size(), "index for Port must be < than Size");

	ConstIterator it;
	int i;
	for (it=mOutPorts.begin(), i=0; it!=mOutPorts.end(); it++, i++)
	{
		if (i==index) return *(*it);
	}
	CLAM_ASSERT(false, "OutPortRegistry::GetByNumber() index out of range");
	
	return *(OutPortBase*)NULL; // just to get rid of warnings
}
	
OutPortBase & OutPortRegistry::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mOutPorts.begin(); it!=mOutPorts.end(); it++)
		if (name == (*it)->GetName()) 
			return **it;
	std::string error( "name not found in OutPorts collection: " );
	error += name;
	CLAM_ASSERT( false, error.c_str() );
	
	return *(OutPortBase*)NULL; // just to get rid of warnings
}
int OutPortRegistry::Size() const
{
	return mOutPorts.size();
}

bool OutPortRegistry::AreReadyForWriting()
{
	Iterator out;
	for ( out=mOutPorts.begin(); out!=mOutPorts.end(); out++)
		if (!(*out)->CanProduce()) return false;
	
	return true;
}

OutPortRegistry::Iterator  OutPortRegistry::Begin()
{
	return mOutPorts.begin();
}

OutPortRegistry::Iterator OutPortRegistry::End()
{
	return mOutPorts.end();
}

OutPortRegistry::ConstIterator  OutPortRegistry::Begin() const
{
	return mOutPorts.begin();
}

OutPortRegistry::ConstIterator OutPortRegistry::End() const
{
	return mOutPorts.end();
}

void OutPortRegistry::Publish( OutPortBase * out )
{
	mOutPorts.push_back( out );
}

}// namespace CLAM
