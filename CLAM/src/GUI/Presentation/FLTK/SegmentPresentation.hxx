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

#ifndef __SEGMENTPRESENTATION__
#define __SEGMENTPRESENTATION__

#include "ProcDataView.hxx"
#include "ProcDataPresentation.hxx"
#include "SegmentView.hxx"
#include "SegmentTFPresentation.hxx"
#include "GeometryKit.hxx"


namespace CLAMGUI
{

	
	template<>
	class ProcDataPresentation<Segment>
		: public Presentation
	{
	public:
		ProcDataPresentation(const char* label = 0)
			: mTimeFreqPres( Geometry( 100, 100, 640, 400 ), label )
			{
				Geometry geo( 100, 100, 640, 400 );
				Init( geo, label );
			}
		ProcDataPresentation( const Geometry& geo, const char* label )
			: mTimeFreqPres( geo, label )
			{
				Init( geo, label );
			}
		virtual ~ProcDataPresentation( )
			{
			}

		void PublishCallbacks();

		void Show();

		void Do();

	protected:
		
		virtual void Init( const Geometry& geo, const char* label );
		

	private:
		
		SegmentTFPresentation      mTimeFreqPres; 
		

	};
	
	
}

#endif // SegmentPresentation.hxx
