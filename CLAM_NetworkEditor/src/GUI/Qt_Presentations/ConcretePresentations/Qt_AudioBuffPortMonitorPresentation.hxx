
#ifndef __Qt_AudioBuffPortMonitorPresentation_hxx__
#define __Qt_AudioBuffPortMonitorPresentation_hxx__

#include "Qt_PortMonitorPresentation.hxx"

namespace NetworkGUI
{

class Qt_AudioBuffPortMonitorPresentation : public Qt_PortMonitorPresentation
{
	Q_OBJECT
public:
	virtual QWidget * SetInnerPlot();
};
	
} // namespace NetworkGUI

#endif //__Qt_AudioBuffPortMonitorPresentation_hxx__  
