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

#ifndef __QT_OUTCONTROLPRESENTATION_HXX__
#define __QT_OUTCONTROLPPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include <qregion.h>
#include "ConnectionPointPresentation.hxx"
#include "Signalv2.hxx"
#include "Signalv1.hxx"

namespace NetworkGUI
{

class Qt_OutControlPresentation : public QWidget, public ConnectionPointPresentation
{
public:
	Qt_OutControlPresentation( int id, QWidget *parent = 0, const char *name = 0);
	virtual void Show();
	virtual void Hide();
	void updatePosition();
	QRegion GetRegion();
	virtual void SetName(const std::string& name);

protected:
	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent *);


public: // signals
	SigSlot::Signalv2< int , int >  SignalAcquirePos;
	SigSlot::Signalv1< Qt_OutControlPresentation * > SignalAcquireOutControlClicked;

protected:
// qt stuff
	int   mId;
	QRegion mReg;
};


} // namespace NetworkGUI

#endif // __QT_OUTCONTROLPRESENTATION_HXX__
