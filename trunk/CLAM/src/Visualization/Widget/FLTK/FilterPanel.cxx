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

#include "FilterPanel.hxx"
#include "GridLayout.hxx"
using namespace CLAMGUI;

LPFPanel::LPFPanel( int x, int y, int w, int h )
{
	mWindow = new Fl_Double_Window( x, y, w, h, "LP Filter" );

	GridLayout gl( 12, 4, Geometry( x, y, w, h ) );

	Geometry coords;

	coords = gl << Geometry( 1, 1, 2, 2 );

	mGain = new GainKnob( coords, "Gain (dB)" );

	coords = gl << Geometry( 4, 1, 2, 2 );

	mLC = new LowcutoffKnob( 22050, coords, "Cutoff(Low) (Hz) ");

	coords = gl << Geometry( 8, 1, 2, 2 );

	mSSB = new SlopeKnob( coords, "Slope(Stop Band)" );

	mWindow->end();

}

void LPFPanel::Show()
{
	mWindow->show();
}

void LPFPanel::Do()
{
	mGain->Do();
	mLC->Do();
	mSSB->Do();

 // mWindow->redraw();
}

BPFPanel::BPFPanel( int x, int y, int w, int h )
{
	mWindow = new Fl_Double_Window( x, y, w, h, "BP filter" );

	GridLayout gl( 20, 4, Geometry( x, y, w, h ) );

	Geometry coords;

	coords = gl << Geometry( 1, 1, 2, 2 );

	mGain = new GainKnob( coords, "Gain (dB)" );

	coords = gl << Geometry( 4, 1, 2, 2 );

	mLC = new LowcutoffKnob( 22050, coords, "Cutoff(Low) (Hz) ");

	coords = gl << Geometry( 8, 1, 2, 2 );

	mSSB = new SlopeKnob( coords, "Slope(Stop Band)" );

	coords = gl << Geometry( 12, 1, 2, 2 );

	mHC = new HighcutoffKnob( 22050, coords, "Cutoff(High) (Hz) ");

	coords = gl << Geometry( 16, 1, 2, 2 );

	mPSB = new SlopeKnob( coords, "Slope(Pass Band)" );


	mWindow->end();

}

void BPFPanel::Show()
{
	mWindow->show();
}

void BPFPanel::Do()
{
	mGain->Do();
	mHC->Do();
	mLC->Do();
	mSSB->Do();
	mPSB->Do();

 // mWindow->redraw();
}

HPFPanel::HPFPanel( int x, int y, int w, int h )
{
	mWindow = new Fl_Double_Window( x, y, w, h, "HP Filter" );

	GridLayout gl( 12, 4, Geometry( x, y, w, h ) );

	Geometry coords;

	coords = gl << Geometry( 1, 1, 2, 2 );

	mGain = new GainKnob( coords, "Gain (dB)" );

	coords = gl << Geometry( 4, 1, 2, 2 );

	mHC = new HighcutoffKnob( 22050, coords, "Cutoff(High) (Hz) ");

	coords = gl << Geometry( 8, 1, 2, 2 );

	mPSB = new SlopeKnob( coords, "Slope(Pass Band)" );

	mWindow->end();
	
}

void HPFPanel::Show()
{
	mWindow->show();
}

void HPFPanel::Do()
{
	mGain->Do();
	mHC->Do();
	mPSB->Do();

 // mWindow->redraw();
}

