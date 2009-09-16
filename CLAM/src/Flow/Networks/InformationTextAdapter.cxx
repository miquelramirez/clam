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


#include "InformationTextAdapter.hxx"
#include "Assert.hxx"
#include "XMLAdapter.hxx"
#include "XMLStorage.hxx"

namespace CLAM
{
	InformationTextAdapter::InformationTextAdapter( int coordX, int coordY, const Text & text)
		: _coordX(coordX)
		,_coordY(coordY)
		,_text(text)
	{}

	InformationTextAdapter::~InformationTextAdapter()
	{}

	void InformationTextAdapter::StoreOn (Storage & store) const
	{
		Text text(_text);

		XMLAdapter<int> coordXAdapter( _coordX, "x", true);
		XMLAdapter<int> coordYAdapter( _coordY, "y", true);
		XMLAdapter<Text> infoAdapter( _text, "text", true);
	
		store.Store(coordXAdapter);
		store.Store(coordYAdapter);
		store.Store(infoAdapter);
	}

	void InformationTextAdapter::LoadFrom (Storage & store) 
	{
		XMLAdapter<int> coordXAdapter( _coordX, "x", true);
		if (not store.Load(coordXAdapter))
			_coordX=0;

		XMLAdapter<int> coordYAdapter( _coordY, "y", true);
		if (not store.Load(coordYAdapter))
			_coordY=0;

		XMLAdapter<Text> infoAdapter( _text, "text", true);
		if (not store.Load(infoAdapter))
			_text=Text("");
	}
} // namespace CLAM

