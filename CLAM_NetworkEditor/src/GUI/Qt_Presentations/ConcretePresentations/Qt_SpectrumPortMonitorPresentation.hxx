
#ifndef __Qt_SpectrumPortMonitorPresentation_hxx__
#define __Qt_SpectrumPortMonitorPresentation_hxx__

#include "Qt_PortMonitorPresentation.hxx"

namespace NetworkGUI
{

class Qt_SpectrumPortMonitorPresentation : public Qt_PortMonitorPresentation
{
	Q_OBJECT
public:
	virtual QWidget * SetInnerPlot();
};
	
} // namespace NetworkGUI

#endif //__Qt_SpectrumPortMonitorPresentation_hxx__  
