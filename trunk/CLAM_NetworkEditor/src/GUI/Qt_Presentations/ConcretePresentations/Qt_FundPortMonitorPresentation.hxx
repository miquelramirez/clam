
#ifndef __Qt_FundPortMonitorPresentation_hxx__
#define __Qt_FundPortMonitorPresentation_hxx__

#include "Qt_ProcessingPresentation.hxx"
#include "DataTypes.hxx"
#include "OutControlSender.hxx"

class QWidget;
namespace CLAMVM {
	class ProcessingController;
}


namespace NetworkGUI
{

class Qt_FundPortMonitorPresentation : public Qt_ProcessingPresentation
{
	Q_OBJECT
	QWidget * mControlRepresentation;
	
public:
	Qt_FundPortMonitorPresentation();
	void UpdateMonitor();
	SigSlot::Slotv1<CLAMVM::ProcessingController&> SlotBindMonitor;
	void BindMonitor(CLAMVM::ProcessingController & controller);
};
	
} // namespace NetworkGUI

#endif //__Qt_FundPortMonitorPresentation_hxx__  
