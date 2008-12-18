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


#include "ConnectionDefinitionAdapter.hxx"
#include "Assert.hxx"
#include "XMLAdapter.hxx"
#include "XMLStorage.hxx"

namespace CLAM
{
	ConnectionDefinitionAdapter::ConnectionDefinitionAdapter( const std::string & out, const std::string & in)
		: mOut(out), mIn(in)
	{
	}

	ConnectionDefinitionAdapter::~ConnectionDefinitionAdapter()
	{
	}

	void ConnectionDefinitionAdapter::StoreOn (Storage & store) const
	{
		Text out(mOut);
		Text in(mIn);
		XMLAdapter<Text> outAdapter( out, "out", true);
		XMLAdapter<Text> inAdapter( in, "in", true);

		store.Store(outAdapter);
		store.Store(inAdapter);
	}

	void ConnectionDefinitionAdapter::LoadFrom (Storage & store) 
	{
		XMLAdapter<Text> outAdapter( mOut, "out", true);
		if (not store.Load(outAdapter))
			throw XmlStorageErr("Failed to read a connection");

		XMLAdapter<Text> inAdapter( mIn, "in", true);	
		if (not store.Load(inAdapter))
			throw XmlStorageErr("Failed to read a connection");
	}
} // namespace CLAM

