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

#ifndef __COMPOSITEVIEW__
#define __COMPOSITEVIEW__

#include <vector>
#include <algorithm>
using std::find;

#include "GView.hxx"
#include "ViewConfiguration.hxx" // Aspect Header

namespace CLAMGUI
{

class CompositeView : 
	public View
{
	typedef std::vector< View* >                 SubViewContainer;

public:
	typedef std::vector< View* >::iterator       ViewIterator;
	typedef std::vector< View* >::const_iterator ConstViewIterator;

public:

	CompositeView()
		: View()
	{
	}

	virtual ~CompositeView()
	{
	}

	virtual const char* GetClassName()
	{
		return "CompositeView";
	}

	virtual void UpdateModel();

	virtual void Release();

	virtual void Refresh();

	ViewIterator FirstView()
	{
		return mChildren.begin();
	}

	ViewIterator LastView()
	{
		return mChildren.end();
	}

	ConstViewIterator FirstView() const
	{
		return mChildren.begin();
	}

	ConstViewIterator LastView() const
	{
		return mChildren.end();
	}

	inline void AddView( View* vw )
	{
		mChildren.push_back( vw );
	}

	inline void RemoveView( View* vw )
	{
		ViewIterator i = find( mChildren.begin(), mChildren.end(), vw );

		mChildren.erase( i );
	}

	virtual void Configure( Aspect& cfg )
	{
		// : TODO : To determine how to handle multiple view configuration
	}

private:

	SubViewContainer   mChildren;

};

}


#endif // CompositeView.hxx
