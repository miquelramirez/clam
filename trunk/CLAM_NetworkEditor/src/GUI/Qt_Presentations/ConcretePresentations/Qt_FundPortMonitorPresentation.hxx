
#ifndef __Qt_FundPortMonitorPresentation_hxx__
#define __Qt_FundPortMonitorPresentation_hxx__

#include "Qt_PortMonitorPresentation.hxx"

namespace NetworkGUI
{

class Qt_FundPortMonitorPresentation : public Qt_PortMonitorPresentation
{
	Q_OBJECT
public:
	virtual QWidget * SetInnerPlot();
};
	
} // namespace NetworkGUI

#endif //__Qt_FundPortMonitorPresentation_hxx__  
