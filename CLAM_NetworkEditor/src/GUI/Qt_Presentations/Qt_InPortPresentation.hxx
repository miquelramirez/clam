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

#ifndef __QT_INPORTPRESENTATION_HXX__
#define __QT_INPORTPPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "InPortPresentation.hxx"
#include "Signalv2.hxx"
#include "Signalv1.hxx"

namespace NetworkGUI
{

class Qt_InPortPresentation : public QWidget, public InPortPresentation
{
public:
	Qt_InPortPresentation( int id, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_InPortPresentation();
	virtual void Show();
	virtual void Hide();
protected:
	virtual void OnNewName(const std::string& name);

	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent *);
public: // signals
	SigSlot::Signalv2< int , int >  AcquirePos;
	SigSlot::Signalv1< Qt_InPortPresentation * > AcquireInPortClicked;

// qt stuff
	int   mId;
};


} // namespace NetworkGUI

#endif // __QT_INPORTPRESENTATION_HXX__
