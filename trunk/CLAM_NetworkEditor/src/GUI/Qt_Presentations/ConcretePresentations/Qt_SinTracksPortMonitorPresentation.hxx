#ifndef __Qt_SinTracksPortMonitorPresentation_hxx__
#define __Qt_SinTracksPortMonitorPresentation_hxx__

#include "Qt_PortMonitorPresentation.hxx"

namespace NetworkGUI
{

class Qt_SinTracksPortMonitorPresentation : public Qt_PortMonitorPresentation
{
	Q_OBJECT
public:
	virtual QWidget * SetInnerPlot();
};
	
} // namespace NetworkGUI

#endif //__Qt_SinTracksPortMonitorPresentation_hxx__  
