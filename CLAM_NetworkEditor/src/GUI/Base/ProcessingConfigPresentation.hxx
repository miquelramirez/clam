
#ifndef __PROCESSINGCONFIGPRESENTATION_HXX__
#define __PROCESSINGCONFIGPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace NetworkGUI
{

class ProcessingConfigPresentation : public CLAMVM::Presentation
{
protected:
//	CLAM::ProcessingConfig * mConfig;

	virtual void OnNewConfig( CLAM::ProcessingConfig* ) = 0;
public:
	ProcessingConfigPresentation();       
	virtual ~ProcessingConfigPresentation(){}
	

public: // signals
	SigSlot::Signalv1< CLAM::ProcessingConfig* > ApplyConfig;
	SigSlot::Slotv1< CLAM::ProcessingConfig* > SetConfig;
	virtual void Show() = 0;
	virtual void Hide() = 0;

};

} // namespace NetworkGUI

#endif // __PROCESSINGCONFIGPRESENTATION_HXX__
