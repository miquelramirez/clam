
#ifndef __Qt_PeaksPortMonitorPresentation_hxx__
#define __Qt_PeaksPortMonitorPresentation_hxx__

#include "Qt_PortMonitorPresentation.hxx"

namespace NetworkGUI
{

class Qt_PeaksPortMonitorPresentation : public Qt_PortMonitorPresentation
{
	Q_OBJECT
public:
	virtual QWidget * SetInnerPlot();
};
	
} // namespace NetworkGUI

#endif //__Qt_PeaksPortMonitorPresentation_hxx__  
