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

#ifndef __OBJECTICON__
#define __OBJECTICON__

#include <string>
#include <FL/Fl_Box.H>
#include "CBL.hxx"

namespace CLAMGUI
{

class ObjectIcon : public Fl_Box
{
public:

	ObjectIcon();

	virtual ~ObjectIcon()
	{
	}

	void resize( int x, int y, int w, int h )
	{
		Fl_Box::resize(x,y,w,h);
	}


	void draw();
	int handle( int event );

	virtual void SetActionCallback( const CBL::Functor1< std::string& >& cb );
	virtual void SetSelectionCallback( const CBL::Functor1< std::string& >& cb );
	void SetObjectName( const char* name );

	void Highlight();
	void Unlight();
	bool IsSelected()
	{
		return mSelected;
	}
	std::string& GetName()
	{
		return mObjectName;
	}

protected:

	void Init();

private:
	CBL::Functor1< std::string& >  mActionCb;
	CBL::Functor1< std::string& >  mSelCb;
	std::string                    mObjectName;
	bool                           mSelected;

};

}

#endif // ObjectIcon.hxx
