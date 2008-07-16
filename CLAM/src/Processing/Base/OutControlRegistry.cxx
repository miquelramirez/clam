/*
 * Copyright (c) 2005 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "OutControlRegistry.hxx"
#include "OutControl.hxx"

namespace CLAM
{

OutControl& OutControlRegistry::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Control must be >=0");
	CLAM_ASSERT(index<Size(), "index for Control must be < than Size");
	
	return *mOutControls[index];
}

OutControl& OutControlRegistry::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mOutControls.begin(); it!=mOutControls.end(); it++)
		if (name == (*it)->GetName()) 
			return **it;
	
	std::string error = 
		"No out control named '" +  name + "'.\nTry with: " + AvailableNames();
	CLAM_ASSERT( false, error.c_str() );

	return *(OutControl*)NULL; // Just to get rid of warnings
}

bool OutControlRegistry::Has(const std::string& name) const
{
	ConstIterator it;
	for (it=mOutControls.begin(); it!=mOutControls.end(); it++)
		if(name == (*it)->GetName()) 
			return true;

	return false;
}

int OutControlRegistry::Size() const
{
	return mOutControls.size();
}

OutControlRegistry::Iterator  OutControlRegistry::Begin()
{
	return mOutControls.begin();
}

OutControlRegistry::Iterator OutControlRegistry::End()
{
	return mOutControls.end();
}

OutControlRegistry::ConstIterator  OutControlRegistry::Begin() const
{
	return mOutControls.begin();
}

OutControlRegistry::ConstIterator OutControlRegistry::End() const
{
	return mOutControls.end();
}

void OutControlRegistry::ProcessingInterface_Register( OutControl * out )
{
	mOutControls.push_back( out );
}

void OutControlRegistry::ProcessingInterface_Unregister( OutControl * out )
{
	for (Iterator it=mOutControls.begin(); it!=mOutControls.end(); it++)
	{
		if (*it==out)
		{
			std::cout << "Removing out control "<< out << std::endl;
			mOutControls.erase(it);
			return;
		}
	}
}	


std::string OutControlRegistry::AvailableNames() const
{
	std::string result;
	std::string separator = "";
	for (ConstIterator it=mOutControls.begin(); it!=mOutControls.end(); it++)
	{
		OutControl & control = *(*it);
		result += separator + "'" + control.GetName() + "'";
		separator = ",";
	}
	return result;
}
} // namespace CLAM

