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

#ifndef __QT_CONNECTIONPRESENTATION_HXX__
#define __QT_CONNECTIONPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include <qpointarray.h>
#include "ConnectionPresentation.hxx"
#include "Slotv2.hxx"

namespace NetworkGUI
{

class Qt_ConnectionPresentation :  public QWidget, public ConnectionPresentation
{
public:
	Qt_ConnectionPresentation( QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_ConnectionPresentation();
	void Show();
	void Hide();
protected:    
	virtual void paintEvent( QPaintEvent * ) = 0;
	void OnNewOutPos(int x, int y);
	void OnNewInPos(int x, int y);
	void UpdatePosition();

	void mousePressEvent( QMouseEvent *);
	void mouseReleaseEvent( QMouseEvent *);
	void keyPressEvent( QKeyEvent * );
	QPoint origin;
	QPoint end;
	bool mDown;
	QPointArray mPositions;

public: //slots
	SigSlot::Slotv2< int, int > SetOutPos;
	SigSlot::Slotv2< int, int > SetInPos;
};

} // namespace NetworkGUI

#endif //__QT_CONNECTIONPRESENTATION_HXX__
