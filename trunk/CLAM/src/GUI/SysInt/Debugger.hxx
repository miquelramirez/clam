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

#ifndef __DEBUGGER__
#define __DEBUGGER__

#include "Breakpoint.hxx"
#include <vector>

namespace CLAMGUI
{

	/**
	*    Singleton class.
	*/
	class DebuggerGUI;
	
	class Debugger
	{
		
		typedef std::vector< Breakpoint* >  ClientVec;
		typedef ClientVec::iterator        Iterator;
		
	public:

		static Debugger* GetInstance()
		{
			if ( mInstance == NULL )
			mInstance = new Debugger;
			
			return mInstance;
		}
		
		DebuggerGUI* GetGUI();

		void RegisterBreakpoint( Breakpoint* bp_ptr )
		{
			mClients.push_back( bp_ptr );
		} 
		
		std::vector<Breakpoint* > GetClients() const
		{
			return mClients;
		}
		
		void Run( unsigned int signaling_bp );
		
		~Debugger();
		
	protected:
		
		void DeactivateBreakpoints();
		
	private:
		
		Debugger();
		
		static Debugger* mInstance;
		ClientVec        mClients;
		unsigned int     mSignalingBp;
		DebuggerGUI*     mGUI;
	};

}

#endif // Debugger.hxx
