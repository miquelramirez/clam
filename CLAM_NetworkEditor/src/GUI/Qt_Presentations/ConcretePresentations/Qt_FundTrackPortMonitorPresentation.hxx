
#ifndef __Qt_FundTrackPortMonitorPresentation_hxx__
#define __Qt_FundTrackPortMonitorPresentation_hxx__

#include "Qt_PortMonitorPresentation.hxx"

namespace NetworkGUI
{

class Qt_FundTrackPortMonitorPresentation : public Qt_PortMonitorPresentation
{
	Q_OBJECT
public:
	virtual QWidget * SetInnerPlot();
};
	
} // namespace NetworkGUI

#endif //__Qt_FundTrackPortMonitorPresentation_hxx__  
