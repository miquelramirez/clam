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

#ifndef __FILTERPANEL__
#define __FILTERPANEL__

#include "FilterControlKnobs.hxx"
#include <FL/Fl_Double_Window.H>

namespace CLAMGUI
{

	class FilterPanel
	{
	public:
		
		virtual GainKnob* GetGainKnob() const
		{
			return NULL;
		}

		virtual LowcutoffKnob* GetLowcutoffKnob() const
		{
			return NULL;
		}

		virtual HighcutoffKnob* GetHighcutoffKnob() const
		{
			return NULL;
		}

		virtual SlopeKnob* GetPassBandSlopeKnob() const
		{
			return NULL;
		}
		
		virtual SlopeKnob* GetStopBandSlopeKnob() const
		{
			return NULL;
		}

		virtual void Show() = 0;

		virtual void Do() = 0;

	};
	
	class LPFPanel
		: public FilterPanel
	{
	public:

		LPFPanel( int x=100, int y=100, int w=300, int h = 100 );

		void Show();

		void Do();

		GainKnob* GetGainKnob() const
		{
			return mGain;
		}

		LowcutoffKnob* GetLowcutoffKnob() const
		{
			return mLC;
		}
		
		SlopeKnob* GetStopBandSlopeKnob() const
		{
			return mSSB;
		}

	private:

		Fl_Window*       mWindow;

		GainKnob*        mGain;
		LowcutoffKnob*   mLC;
		SlopeKnob*       mSSB;
	};

	class BPFPanel
		: public FilterPanel

	{
	public:

		BPFPanel( int x=100, int y=100, int w=500, int h = 100 );

		void Show();

		void Do();

		GainKnob* GetGainKnob() const
		{
			return mGain;
		}

		LowcutoffKnob* GetLowcutoffKnob() const
		{
			return mLC;
		}

		HighcutoffKnob* GetHighcutoffKnob() const
		{
			return mHC;
		}

		SlopeKnob* GetPassBandSlopeKnob() const
		{
			return mPSB;
		}

		SlopeKnob* GetStopBandSlopeKnob() const
		{
			return mSSB;
		}

	private:

		Fl_Window*       mWindow;

		GainKnob*        mGain;
		HighcutoffKnob*  mHC;
		LowcutoffKnob*   mLC;
		SlopeKnob*       mPSB;
		SlopeKnob*       mSSB;    
	};
	
	class HPFPanel
		: public FilterPanel
	{
	public:

		HPFPanel( int x=100, int y=100, int w=300, int h = 100 );

		void Show();

		void Do();

		GainKnob* GetGainKnob() const
		{
			return mGain;
		}

		HighcutoffKnob* GetHighcutoffKnob() const
		{
			return mHC;
		}

		SlopeKnob* GetPassBandSlopeKnob() const
		{
			return mPSB;
		}

	 private:

		Fl_Window*       mWindow;

		GainKnob*        mGain;
		HighcutoffKnob*  mHC;
		SlopeKnob*       mPSB;
	};


}

#endif // FilterPanel.hxx
