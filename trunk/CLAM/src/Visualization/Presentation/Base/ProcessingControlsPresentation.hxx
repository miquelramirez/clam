#ifndef __PROCESSINGCONTROLSPRESENTATION__
#define __PROCESSINGCONTROLSPRESENTATION__

#include "Presentation.hxx"
#include "ProcessingInControlSet.hxx"

namespace CLAMVM
{
	class ProcessingInControlsPresentation
	{
	public:
		virtual ~ProcessingInControlsPresentation();
			
		virtual void Show() = 0;
		virtual void Hide() = 0;
			
		virtual void AttachTo( ProcessingInControlSet& set ) = 0;
		virtual void Detach() = 0;
	};
}

#endif // ProcessingControlsPresentation.hxx
