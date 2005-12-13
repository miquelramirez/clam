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

#ifndef __QT_NETWORKPRESENTATION_HXX__
#define __QT_NETWORKPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "NetworkPresentation.hxx"
#include <CLAM/Signalv1.hxx>
#include <CLAM/Signalv0.hxx>
#include <CLAM/Slotv1.hxx>

namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class ProcessingController;
//	class ConnectionAdapter;
}

namespace NetworkGUI
{

class Qt_PortConnectionPresentation;
class Qt_ControlConnectionPresentation;
class Qt_ConnectionPresentation;
class Qt_InPortPresentation;
class Qt_OutPortPresentation;
class Qt_InControlPresentation;
class Qt_OutControlPresentation;
class Qt_ProcessingPresentation;
class MainWindow;

class Qt_NetworkPresentation :  public QWidget, public NetworkPresentation
{
	MainWindow * mMainWindow;
	
	typedef std::list<Qt_ProcessingPresentation * > QtProcessingList;
	typedef std::list<Qt_ConnectionPresentation * > QtConnectionList;
	
	void AttachConnectionToPortPresentations( Qt_PortConnectionPresentation * );
	void AttachConnectionToControlPresentations( Qt_ControlConnectionPresentation * );
	void SetName(const std::string& name); 
	void CreateProcessingPresentation( const std::string &, CLAMVM::ProcessingController * );
	void CreatePortConnectionPresentation( const std::string &, const std::string & );
	void CreateControlConnectionPresentation( const std::string &,const std::string & );
	
	void SetInPortClicked( Qt_InPortPresentation *);
	void SetOutPortClicked( Qt_OutPortPresentation *);
	void SetInControlClicked( Qt_InControlPresentation *);
	void SetOutControlClicked( Qt_OutControlPresentation *);

	void ProcessingPresentationSelected( Qt_ProcessingPresentation * );
	void ConnectionPresentationSelected( Qt_ConnectionPresentation * );
	void ProcessingPresentationAddedToSelection( Qt_ProcessingPresentation * );
	void ConnectionPresentationAddedToSelection( Qt_ConnectionPresentation * );

	void keyPressEvent( QKeyEvent * );
	void paintEvent( QPaintEvent * );
	void mouseMoveEvent( QMouseEvent *);
	void mouseReleaseEvent( QMouseEvent *);
	void mousePressEvent ( QMouseEvent *); 
	void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
	void MovingMouseWithButtonPressed( const QPoint & );
	void DrawSelectionRectangle();
	void CheckSelectionRectangle();
	bool CheckPortsSelection( QMouseEvent *m );
	bool CheckControlsSelection(QMouseEvent *m );

	/** Used in SetUpWidgetsPositions: queries names readen from a file*/
	Qt_ProcessingPresentation* FindProcessingPresentation(const std::string& name);
		
	void SendMessageToStatus( const std::string & );		
	
	Qt_InPortPresentation* mInPortSelected;
	Qt_OutPortPresentation* mOutPortSelected;
	Qt_InControlPresentation* mInControlSelected;
	Qt_OutControlPresentation* mOutControlSelected;
	QPoint mMousePos;
	QPoint mSelectionPos;

	QtProcessingList mSelectedProcessingPresentations;
	QtConnectionList mSelectedConnectionPresentations;
public:
	Qt_NetworkPresentation( MainWindow *parent = 0, const char *name = 0);
	void Show();
	void Hide();

	void SetUpWidgetsPositions(const std::string& positionsFilename);
	void SaveWidgetsPositions(const std::string& positionsFilename);


	// slots
	SigSlot::Slotv1< Qt_InPortPresentation * > SlotSetInPortClicked;
	SigSlot::Slotv1< Qt_OutPortPresentation * > SlotSetOutPortClicked;
	SigSlot::Slotv1< Qt_InControlPresentation * > SlotSetInControlClicked;
	SigSlot::Slotv1< Qt_OutControlPresentation * > SlotSetOutControlClicked;

	SigSlot::Slotv1< Qt_ProcessingPresentation * > SlotProcessingPresentationSelected;
	SigSlot::Slotv1< Qt_ProcessingPresentation * > SlotProcessingPresentationAddedToSelection;
	SigSlot::Slotv1< Qt_ConnectionPresentation * > SlotConnectionPresentationSelected;
	SigSlot::Slotv1< Qt_ConnectionPresentation * > SlotConnectionPresentationAddedToSelection;

	SigSlot::Slotv1< const std::string & > SlotSendMessageToStatus;
	SigSlot::Slotv1< const QPoint & > SlotMovingMouseWithButtonPressed;

	// signals
	SigSlot::Signalv1< const QPoint & > SignalAcquireOutPortAfterClickInPort;
	SigSlot::Signalv1< const QPoint & > SignalAcquireInPortAfterClickOutPort;
	SigSlot::Signalv1< const QPoint & > SignalAcquireOutControlAfterClickInControl;
	SigSlot::Signalv1< const QPoint & > SignalAcquireInControlAfterClickOutControl;
	SigSlot::Signalv1< const std::string& > SignalSendMessageToStatus;
	SigSlot::Signalv0 SignalProcessingCreated;

private:
	const std::string GetCompleteNameFromInPortSelected();
	const std::string GetCompleteNameFromOutPortSelected();
	const std::string GetCompleteNameFromInControlSelected();
	const std::string GetCompleteNameFromOutControlSelected();

};

} // namespace NetworkGUI

#endif //__QT_NETWORKPRESENTATION_HXX__
