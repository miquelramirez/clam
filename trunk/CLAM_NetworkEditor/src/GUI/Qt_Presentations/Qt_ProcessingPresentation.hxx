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

#ifndef __Qt_ProcessingPresentation_hxx__
#define __Qt_ProcessingPresentation_hxx__

#include <string>
#include <qwidget.h>
#include "ProcessingPresentation.hxx"
#include "Signalv1.hxx"
#include "Signalv0.hxx"
#include "Slotv1.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace NetworkGUI
{

class Qt_InPortPresentation; 
class Qt_OutPortPresentation; 
class Qt_InControlPresentation; 
class Qt_OutControlPresentation; 

class Qt_ProcessingPresentation : public QWidget, public ProcessingPresentation
{
	Q_OBJECT
public:
	Qt_ProcessingPresentation( std::string nameFromNetwork, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_ProcessingPresentation(){}
	void Show();
	void Hide();
	void EmitPositionOfChildren();

	void UnSelectProcessingPresentation();
	void Move( const QPoint & );
protected:
	virtual void SetObservedClassName(const std::string& name);

	// port methods
	void SetInPort( const std::string & );
	void SetOutPort( const std::string & );
	void SetInPortClicked( Qt_InPortPresentation *);
	void SetOutPortClicked( Qt_OutPortPresentation *);
	void SetOutPortAfterClickInPort(const QPoint &);
	void SetInPortAfterClickOutPort(const QPoint &);

	// control methods
	void SetInControl( const std::string & );
	virtual void SetOutControl( const std::string & ); // reimplemented in Qt_OutControlSender widget
	void SetInControlClicked( Qt_InControlPresentation *);
	void SetOutControlClicked( Qt_OutControlPresentation *);
	void SetOutControlAfterClickInControl(const QPoint &);
	void SetInControlAfterClickOutControl(const QPoint &);
	

	virtual void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void mouseDoubleClickEvent ( QMouseEvent * );

	void ConfigurationUpdated( bool );
	void UpdateOutPortsPosition();
	void UpdateOutControlsPosition();

	virtual void UpdateSize();
	void ChangeProcessingPresentationName( const std::string & name ); // redefinition to let update the presentation
	void DrawSelectedRepresentation();
	void  UpdatePresentation();
	bool	    mSelected;
	QPoint     mPrevPos;

public: // signals
	SigSlot::Signalv1< Qt_InPortPresentation * > SignalAcquireInPortClicked;
	SigSlot::Signalv1< Qt_OutPortPresentation * > SignalAcquireOutPortClicked;
	SigSlot::Signalv1< Qt_InControlPresentation * > SignalAcquireInControlClicked;
	SigSlot::Signalv1< Qt_OutControlPresentation * > SignalAcquireOutControlClicked;

	SigSlot::Signalv1< Qt_ProcessingPresentation * > SignalProcessingPresentationSelected;
	SigSlot::Signalv1< Qt_ProcessingPresentation * > SignalProcessingPresentationAddedToSelection;
	SigSlot::Signalv1< const std::string & > SignalSendMessageToStatus;
	SigSlot::Signalv1< const QPoint & > SignalMovingMouseWithButtonPressed;

public: // slots
	// ports
	SigSlot::Slotv1< Qt_InPortPresentation * > SlotSetInPortClicked;
	SigSlot::Slotv1< Qt_OutPortPresentation * > SlotSetOutPortClicked;
	SigSlot::Slotv1< const QPoint & > SlotSetOutPortAfterClickInPort;
	SigSlot::Slotv1< const QPoint & > SlotSetInPortAfterClickOutPort;

	//controls
	SigSlot::Slotv1< Qt_InControlPresentation * > SlotSetInControlClicked;
	SigSlot::Slotv1< Qt_OutControlPresentation * > SlotSetOutControlClicked;
	SigSlot::Slotv1< const QPoint & > SlotSetOutControlAfterClickInControl;
	SigSlot::Slotv1< const QPoint & > SlotSetInControlAfterClickOutControl;

	// configure related
	SigSlot::Slotv1< bool > SlotConfigurationUpdated;

public slots: // qt slots
	void SlotTextChange( const QString & );
	void SlotExecuteChangeName();
signals:
	void SignalEmitGeometryChange( const QRect & );
};


} // namespace NetworkGUI

#endif // __Qt_ProcessingPresentation_hxx__

