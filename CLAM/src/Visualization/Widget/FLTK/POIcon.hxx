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

#ifndef __POICON__
#define __POICON__

#include "ObjectIcon.hxx"
namespace CLAM {
	class Processing;
}


namespace CLAMGUI
{


class POIcon : public ObjectIcon
{

public:
	virtual ~POIcon() {};

	int handle( int event );

	void SetAvatar( CLAM::Processing* p_pobj);

	void SetActionCallback( const CBL::Functor1< CLAM::Processing* >& cb );

private:

	CBL::Functor1< CLAM::Processing* >   mActionCb;
	CLAM::Processing*                    mAvatar;

};

}


#endif // POIcon.hxx
