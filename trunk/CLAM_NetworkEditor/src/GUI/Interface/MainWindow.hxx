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

#ifndef __MainWindow_hxx_
#define __MainWindow_hxx_

#include <qmainwindow.h> 
#include <string>
#include "Slotv1.hxx"
#include "Signalv1.hxx"
#include "Signalv0.hxx"
#include "Qt_NetworkPresentation.hxx"

class QDockWindow;

namespace NetworkGUI
{

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	virtual ~MainWindow();
	Qt_NetworkPresentation & GetNetworkPresentation();

private:
	void OnNewMessageToStatus( const std::string & );
	void Start();
	void Stop();

	Qt_NetworkPresentation mNetwork;
	QDockWindow * mDockProcMenu;

public: //slots
	SigSlot::Slotv1< const std::string & > SlotNewMessageToStatus;

 public slots:
 void LoadNetwork();
	void SaveNetwork();
	void NewNetwork(); 
	void SaveAsNetwork();
	void StartNetwork();
	void StopNetwork();
	void ShowProcMenu();

public: // CLAM signals
	SigSlot::Signalv1 < bool > SignalChangeNetworkState;
	SigSlot::Signalv0 SignalNewNetworkSignal;

};

} // namespace NetworkGUI

#endif // __MainWindow_hxx_
