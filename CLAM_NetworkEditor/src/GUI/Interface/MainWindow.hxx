
#ifndef __MainWindow_hxx_
#define __MainWindow_hxx_

#include <qmainwindow.h> 
#include <string>
#include "Slotv1.hxx"
#include "Qt_NetworkPresentation.hxx"
#include "FactoryToolBox.hxx"
#include "NetworkActions.hxx"

namespace NetworkGUI
{

class MainWindow : public QMainWindow
{
public:
	MainWindow();
	virtual ~MainWindow();
	Qt_NetworkPresentation & GetNetworkPresentation();

private:
	void OnNewMessageToStatus( const std::string & );
	void OnNewChangeOnNetworkState( bool );
	void Start();
	void Stop();

	Qt_NetworkPresentation mNetwork;
	FactoryToolBox mFactory;
	NetworkActions mNetworkActions;

public: //slots
	SigSlot::Slotv1< const std::string & > NewMessageToStatus;

};

} // namespace NetworkGUI

#endif // __MainWindow_hxx_
