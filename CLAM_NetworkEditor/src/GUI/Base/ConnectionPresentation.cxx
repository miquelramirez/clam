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

#include "ConnectionPresentation.hxx"
#include "ConnectionAdapter.hxx"
#include "Processing.hxx"

namespace NetworkGUI
{

ConnectionPresentation::ConnectionPresentation()
	: mInName( "unnamed inport" ),
	  mOutName( "unnamed outport" )
{
	SlotSetNames.Wrap( this, &ConnectionPresentation::SetNames );
}

ConnectionPresentation::~ConnectionPresentation()
{
}

void ConnectionPresentation::AttachTo( CLAMVM::ConnectionAdapter & adapter )
{
	adapter.SignalAcquireNames.Connect(SlotSetNames);
}

void ConnectionPresentation::SetNames( const std::string & portOut, const std::string & portIn )
{
	mInName = portIn;
	mOutName = portOut;
}

} // namespace NetworkGUI
