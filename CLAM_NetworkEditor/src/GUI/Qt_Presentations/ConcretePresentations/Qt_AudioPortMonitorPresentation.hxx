
#ifndef __Qt_AudioPortMonitorPresentation_hxx__
#define __Qt_AudioPortMonitorPresentation_hxx__

#include "Qt_PortMonitorPresentation.hxx"

namespace NetworkGUI
{

class Qt_AudioPortMonitorPresentation : public Qt_PortMonitorPresentation
{
	Q_OBJECT
public:
	virtual QWidget * SetInnerPlot();
};
	
} // namespace NetworkGUI

#endif //__Qt_AudioPortMonitorPresentation_hxx__  
