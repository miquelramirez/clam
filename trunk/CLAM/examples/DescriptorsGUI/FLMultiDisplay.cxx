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

#include "FLMultiDisplay.hxx"
#include "ErrGUI.hxx"
#include <FL/fl_draw.H>
#include <cstddef>

using namespace CLAMGUI;

FLMultiDisplay::FLMultiDisplay(int x,int y,int w,int h)
	: FLDisplay(x,y,w,h), mChildren(x,y,w,h)
{
}

void FLMultiDisplay::draw(void)
{
	fl_push_clip( x(), y(), w(), h());
	fl_color(color());
	fl_rectf(x(),y(),w(),h());
	for (int i=0;i<mChildren.children();i++)
	{
		FLDisplay* d = dynamic_cast< FLDisplay* > ( mChildren.child(i) );

		if (d == NULL )
		{
			CLAMGUI::GLPort* port = dynamic_cast < CLAMGUI::GLPort* >( mChildren.child(i) );

			if ( port == NULL )
				throw ( GUIException( "Pointer was messed up "  ) );
			else
			{
				port->mInMultiDisplay = this;
				port->mHorRange = mHorRange;
				port->mVerRange = mVerRange;

				port->DrawSelf();
			}
		}
		else
		{
			d->mInMultiDisplay = this;

			d->mHorRange = mHorRange;
			d->mVerRange = mVerRange;

			d->DrawSelf();
		}
	}
	fl_pop_clip();
}









