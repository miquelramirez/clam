
/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __ACTION_BUTTON_HXX__
#define __ACTION_BUTTON_HXX__

// XR: Temporary class used until qt signal/slots and preprocessor macros (mocs) are supported by clam build system


#include <qpushbutton.h>
#include "Signalv1.hxx"

namespace NetworkGUI
{

class ActionButton : public QPushButton	
{
public:
	ActionButton( QWidget * parent = 0, const char *name = 0 );
protected:
	virtual void mousePressEvent ( QMouseEvent * );

public: //signals
	SigSlot::Signalv1 < bool > Pressed;
};

} // namespace NetworkGUI

#endif // __ACTION_BUTTON_HXX__

