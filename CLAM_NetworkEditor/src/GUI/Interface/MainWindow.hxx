
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
//	void OnNewChangeOnNetworkState( bool );
	void Start();
	void Stop();

	Qt_NetworkPresentation mNetwork;
	QDockWindow * mDockProcMenu;

public: //slots
	SigSlot::Slotv1< const std::string & > NewMessageToStatus;

 public slots:
 void LoadNetwork();
	void SaveNetwork();
	void NewNetwork(); 
	void SaveAsNetwork();
	void StartNetwork();
	void StopNetwork();
	void ShowProcMenu();

public: // CLAM signals
	SigSlot::Signalv1 < bool > ChangeNetworkState;
	SigSlot::Signalv0 NewNetworkSignal;

};

} // namespace NetworkGUI

#endif // __MainWindow_hxx_
