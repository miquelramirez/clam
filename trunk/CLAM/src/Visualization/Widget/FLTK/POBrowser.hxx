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

#ifndef __POBROWSER__
#define __POBROWSER__

#include "Panel.hxx"
#include <FL/Fl_Double_Window.H>
#include "ObjectScroller.hxx"
#include "GeometryKit.hxx"
#include <string>
using std::string;

namespace CLAMGUI
{

class PORepository;

class POBrowser : public Panel
{
public:

	POBrowser();

	virtual ~POBrowser();
	
	void SetType( const std::string& s );
	const std::string& GetType() const
	{
		return mPOType;
	}
	void BuildWidgets( const Rect<int>& scrspace );
	void HideAll();
	void ShowAll();

protected:

	void ShowPresentPO();

public:

	Fl_Double_Window*    mWindow;
	ObjectScroller*      mScroll;
	string               mPOType;
	PORepository*        mRepository;
};

}

#endif // POBrowser.hxx
