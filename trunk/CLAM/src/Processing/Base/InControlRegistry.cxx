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

#include "InControlRegistry.hxx"
#include "InControl.hxx"

namespace CLAM
{

InControl& InControlRegistry::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Control must be >=0");
	CLAM_ASSERT(index<Size(), "index for Control must be < than Size");
	return *mInControls.at(index);
}

InControl& InControlRegistry::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mInControls.begin(); it!=mInControls.end(); it++)
		if (name == (*it)->GetName()) 
			return **it;

	std::string error( "name not found in InControls collection: " );
	error += "'" +  name + "'" + ". In ports availables: " + AvailableNames();
	CLAM_ASSERT( false, error.c_str() );

	return *(InControl*)NULL; // just to get rid of warnings
}
std::string InControlRegistry::AvailableNames() const
{
	std::string result;
	ConstIterator it;
	bool first=true;
	for (it=mInControls.begin(); it!=mInControls.end(); it++)
	{
		if (!first) 
			result += ", ";
		else 
			first = false;
		InControl & control = *(*it);
		result += "'";
		result += control.GetName();
		result += "'";
	}
	return result;
}

int InControlRegistry::Size() const
{
	return mInControls.size();
}

InControlRegistry::Iterator  InControlRegistry::Begin()
{
	return mInControls.begin();
}

InControlRegistry::Iterator InControlRegistry::End()
{
	return mInControls.end();
}

InControlRegistry::ConstIterator  InControlRegistry::Begin() const
{
	return mInControls.begin();
}

InControlRegistry::ConstIterator InControlRegistry::End() const
{
	return mInControls.end();
}

void InControlRegistry::Publish( InControl * in )
{
	mInControls.push_back( in );
}
		
} // namespace CLAM

