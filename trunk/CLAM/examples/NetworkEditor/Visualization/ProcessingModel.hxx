

#ifndef _PROCESSINGMODEL_
#define _PROCESSINGMODEL_

#include "Signalv1.hxx"
#include <string>

namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class InPortAdapter;
	class OutPortAdapter;
	class InControlAdapter;
	class OutControlAdapter;

	class ProcessingModel
	{
	public:		
		ProcessingModel(){}
		
		virtual ~ProcessingModel(){}
		//signals
//		SigSlot::Signalv1< const std::string &>          AcquireName;
		SigSlot::Signalv1< CLAM::ProcessingConfig *>  AcquireConfig;
		SigSlot::Signalv1< const std::string &>          AcquireClassName;
		SigSlot::Signalv1< CLAMVM::InPortAdapter* >   AcquireInPort;
		SigSlot::Signalv1< CLAMVM::OutPortAdapter* > AcquireOutPort;
		SigSlot::Signalv1< CLAMVM::InControlAdapter* >   AcquireInControl;
		SigSlot::Signalv1< CLAMVM::OutControlAdapter* > AcquireOutControl;
		SigSlot::Slotv1< CLAM::ProcessingConfig * > SetNewConfig;
	};
}

#endif // _PROCESSINGMODEL_
