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

#ifndef __QT_PROCESSINGPRESENTATION_HXX__
#define __QT_PROCESSINGPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "ProcessingPresentation.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class InPortAdapter;
	class OutPortAdapter;
	class InControlAdapter;
	class OutControlAdapter;
}

namespace NetworkGUI
{

class Qt_InPortPresentation; 
class Qt_OutPortPresentation; 
class Qt_InControlPresentation; 
class Qt_OutControlPresentation; 

class Qt_ProcessingPresentation : public QWidget, public ProcessingPresentation
{
public:
	Qt_ProcessingPresentation( std::string nameFromNetwork, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_ProcessingPresentation();
	virtual void Show();
	virtual void Hide();
	void EmitPositionOfChildren();
protected:
	virtual void OnNewObservedClassName(const std::string& name);

	// port methods
	virtual void OnNewInPort( CLAMVM::InPortAdapter* );
	virtual void OnNewOutPort( CLAMVM::OutPortAdapter* );	
	virtual void OnNewInPortClicked( Qt_InPortPresentation *);
	virtual void OnNewOutPortClicked( Qt_OutPortPresentation *);
	virtual void OnNewOutPortAfterClickInPort(const QPoint &);
	virtual void OnNewInPortAfterClickOutPort(const QPoint &);

	// control methods
	virtual void OnNewInControl( CLAMVM::InControlAdapter* );
	virtual void OnNewOutControl( CLAMVM::OutControlAdapter* );	
	virtual void OnNewInControlClicked( Qt_InControlPresentation *);
	virtual void OnNewOutControlClicked( Qt_OutControlPresentation *);
	virtual void OnNewOutControlAfterClickInControl(const QPoint &);
	virtual void OnNewInControlAfterClickOutControl(const QPoint &);


	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void keyPressEvent( QKeyEvent * );
	void updateOutPortsPosition();
	void updateOutControlsPosition();

// qt stuff
	bool        mDown;
	QPoint     mClickPos;
//	Qt_ProcessingConfigPresentation*  mpConfigPresentation;	

public: // signals
	SigSlot::Signalv1< Qt_InPortPresentation * > AcquireInPortClicked;
	SigSlot::Signalv1< Qt_OutPortPresentation * > AcquireOutPortClicked;
	SigSlot::Signalv1< Qt_InControlPresentation * > AcquireInControlClicked;
	SigSlot::Signalv1< Qt_OutControlPresentation * > AcquireOutControlClicked;
//	SigSlot::Signalv1< CLAM::ProcessingConfig * > EditConfiguration;

public: // slots
	// ports
	SigSlot::Slotv1< Qt_InPortPresentation * > SetInPortClicked;
	SigSlot::Slotv1< Qt_OutPortPresentation * > SetOutPortClicked;
	SigSlot::Slotv1< const QPoint & > SetOutPortAfterClickInPort;
	SigSlot::Slotv1< const QPoint & > SetInPortAfterClickOutPort;

	//controls
	SigSlot::Slotv1< Qt_InControlPresentation * > SetInControlClicked;
	SigSlot::Slotv1< Qt_OutControlPresentation * > SetOutControlClicked;
	SigSlot::Slotv1< const QPoint & > SetOutControlAfterClickInControl;
	SigSlot::Slotv1< const QPoint & > SetInControlAfterClickOutControl;

};


} // namespace NetworkGUI

#endif // __QT_PROCESSINGPRESENTATION_HXX__
