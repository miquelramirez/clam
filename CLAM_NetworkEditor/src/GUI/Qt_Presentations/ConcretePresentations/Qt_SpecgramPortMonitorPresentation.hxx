
#ifndef __Qt_SpecgramPortMonitorPresentation_hxx__
#define __Qt_SpecgramPortMonitorPresentation_hxx__

#include "Qt_PortMonitorPresentation.hxx"

namespace NetworkGUI
{

class Qt_SpecgramPortMonitorPresentation : public Qt_PortMonitorPresentation
{
	Q_OBJECT
public:
	virtual QWidget * SetInnerPlot();
};
	
} // namespace NetworkGUI

#endif //__Qt_SpecgramPortMonitorPresentation_hxx__  
