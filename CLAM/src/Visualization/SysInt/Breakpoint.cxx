/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Breakpoint.hxx"
#include "Debugger.hxx"

using namespace CLAMGUI;

unsigned int Breakpoint::mInstanceCounter = 0;

Breakpoint::Breakpoint()
	: mIsActive( false ), mName( "A Breakpoint" ), mDbgInst( NULL ), mBroke( false )
{
	mGUID = mInstanceCounter;
	mInstanceCounter++;
}

void Breakpoint::SetName( const char* name )
{
	mName = std::string( name );
}

void Breakpoint::Activate()
{
	mIsActive = true;
	mDbgInst = Debugger::GetInstance();
	mDbgInst->RegisterBreakpoint( this );

}

void Breakpoint::Deactivate()
{
	mIsActive = false;
	//  mDbgInst->DeregisterBreakpoint( this );
}

void Breakpoint::Break()
{
	mBroke = true;
	if ( mDbgInst!= NULL && mIsActive )
		mDbgInst->Run( mGUID );
	mBroke = false;
}
