/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __AUDIOPLOT__
#define __AUDIOPLOT__

#include "PlotBase.hxx"

namespace CLAM
{
	class Audio;
}

namespace CLAMVM
{
	class AudioAdapter;
	class Fl_SMS_Browsable_Playable_Audio;
	

	class AudioPlot : public PlotBase
	{
	public:
		AudioPlot();
		AudioPlot( std::string name );
		virtual ~AudioPlot();

		void SetPosition( int x, int y );
		void SetSize( int w, int h );
		void SetLabel( const char* label );
		void SetYRange( double ymin, double ymax );
		void SetTooltipFormat( const char* tooltipFormat );
		
		void SetData( const CLAM::Audio& obj );

	protected:

		AudioAdapter*                      mpAdapter;
		Fl_SMS_Browsable_Playable_Audio*   mpWidget;
	};

}


#endif // AudioPlot.hxx
