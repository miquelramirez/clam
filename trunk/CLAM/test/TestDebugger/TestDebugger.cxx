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

#include "DummySystem.hxx"
#include "Debugger.hxx"
#include "DebuggerGUI.hxx"
#include "Breakpoint.hxx"
#include <iostream>
using std::cout;
using std::endl;
using namespace CLAM;

int main( int argc, char** argv )
{
	bool done = false;
	Breakpoint bp0, bp1, bp2, bp3, bp4, bp5;

	int k = 10;

	SpectralDelaySystem s;

	s.InitProcessings();

	cout << "Debugger Test Start...." << endl;

	bp0.Activate(); bp0.SetName( "Breakpoint #1" );
	bp1.Activate(); bp1.SetName( "Breakpoint #2" );
	bp2.Activate(); bp2.SetName( "Breakpoint #3" );
	bp3.Activate(); bp3.SetName( "Breakpoint #4" );
	bp4.Activate(); bp4.SetName( "Breakpoint #5" );
	bp5.Activate(); bp5.SetName( "Breakpoint #6" );

	// Dummy Main Loop;
	while ( k > 0 )
	{
		cout << " Some snippet of user code #1 " << endl;
		bp0.Break();
		cout << " Some snippet of user code #2 " << endl;
		bp1.Break();
		cout << " Some snippet of user code #3 " << endl;
		bp2.Break();
		cout << " Some snippet of user code #4 " << endl;
		bp3.Break();
		cout << " Some snippet of user code #5 " << endl;
		bp4.Break();
		cout << " Some snippet of user code #6 " << endl;
		bp5.Break();
		k--;
		cout << " Some more user code...." << endl;
		cout << " Iterations left: " << k << endl;
	}

	bp0.Deactivate();
	bp1.Deactivate();
	bp2.Deactivate();
	bp3.Deactivate();
	bp4.Deactivate();
	bp5.Deactivate();

	cout << "Debugger Test End..." << endl;

	return 0;
}
