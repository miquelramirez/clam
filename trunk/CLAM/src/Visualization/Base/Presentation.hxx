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

#ifndef __PRESENTATION__
#define __PRESENTATION__

#include "GView.hxx"
#include "GeometryKit.hxx"
#include "Assert.hxx"

namespace CLAMGUI
{

class Presentation
{

public:

	Presentation()
	{
	}


	virtual void LinkWithView( View* v )
	{
		mLinkedView = v;

		PublishCallbacks();
	}

	View* GetLinkedView(void)
	{
		return mLinkedView;
	}

	virtual void PublishCallbacks()
	{
		CLAM_ASSERT( false, " Base presentation class PublishCallbacks() invoked. It is mandatory to overwrite this method" );
	}

	virtual ~Presentation()
	{
		mLinkedView->Release();
	}

protected:

	virtual void Init( const Geometry& g, const char* label = 0 )
	{
	}

	View* mLinkedView;

};

}

#endif // Presentation.hxx
