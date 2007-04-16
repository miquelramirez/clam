/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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



#ifndef __CONNECTIONDEFINITIONADAPTER_HXX__
#define __CONNECTIONDEFINITIONADAPTER_HXX__

#include "Component.hxx"
#include "Text.hxx"
#include <string>

namespace CLAM
{

class ConnectionDefinitionAdapter :public Component
{
private:
	Text mOut;
	Text mIn;

public:
	ConnectionDefinitionAdapter( const std::string & out = "", const std::string & in = "");
	virtual ~ConnectionDefinitionAdapter();	
	const std::string & GetOutName(){return mOut;}
	const std::string & GetInName(){return mIn;}

public:
	//* Returns the class name
	const char * GetClassName() const 
	{
		return "ConnectionDefinitionAdapter";
	}
	
	virtual void StoreOn (Storage & store) const;

	/**
	 * Loads component's subitems from the given Storage
	 * @param store The given storage where the subitem will be loaded from
	 * @see Storage
	 */
	virtual void LoadFrom (Storage & store);
};

} // namespace CLAM

#endif // __CONNECTIONDEFINITIONADAPTER_HXX__

