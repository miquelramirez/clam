
#ifndef __Qt_PortMonitorPresentation_hxx__
#define __Qt_PortMonitorPresentation_hxx__

#include "Qt_ProcessingPresentation.hxx"
#include <CLAM/DataTypes.hxx>
#include <CLAM/OutControlSender.hxx>

#include <CLAM/NetPeaksPlot.hxx>

class QWidget;
namespace CLAMVM {
	class ProcessingController;
}


namespace NetworkGUI
{

class Qt_PortMonitorPresentation : public Qt_ProcessingPresentation
{
	Q_OBJECT
public:
	Qt_PortMonitorPresentation();
	void ProcessingControllerAttached();
	virtual QWidget * SetInnerPlot() = 0;
	template <typename ConcretePlot>
	QWidget * SetConcreteInnerPlot()
	{
		typedef typename ConcretePlot::MonitorType ConcreteMonitor;
		ConcretePlot * innerPlot = new ConcretePlot(this);
		CLAM_ASSERT(mController,"No Controller attached");
		ConcreteMonitor & monitor = 
			dynamic_cast<ConcreteMonitor &>(mController->GetObserved());
		innerPlot->SetMonitor(monitor);
		return innerPlot;
	}
};

} // namespace NetworkGUI

#endif //__Qt_PortMonitorPresentation_hxx__  
