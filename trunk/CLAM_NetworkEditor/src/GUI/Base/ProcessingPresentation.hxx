
#ifndef __PROCESSINGPRESENTATION_HXX__
#define __PROCESSINGPRESENTATION_HXX__


#include <list>
#include <string>

#include "Presentation.hxx"
#include "ProcessingConfigPresentation.hxx"
#include "Slotv1.hxx"
#include "Signalv1.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class ProcessingModel;
	class InPortAdapter;
	class OutPortAdapter;
	class InControlAdapter;
	class OutControlAdapter;
}

namespace NetworkGUI
{

	class InPortPresentation;
	class OutPortPresentation;
	class InControlPresentation;
	class OutControlPresentation;

class ProcessingPresentation : public CLAMVM::Presentation
{
protected:
	std::list<InPortPresentation*> mInPortPresentations;
	std::list<OutPortPresentation*> mOutPortPresentations;
	std::list<InControlPresentation*> mInControlPresentations;
	std::list<OutControlPresentation*> mOutControlPresentations;
	typedef std::list<InPortPresentation*>::iterator InPortPresentationIterator;
	typedef std::list<OutPortPresentation*>::iterator OutPortPresentationIterator;
	typedef std::list<InControlPresentation*>::iterator InControlPresentationIterator;
	typedef std::list<OutControlPresentation*>::iterator OutControlPresentationIterator;
	std::string mNameFromNetwork;
	std::string mObservedClassName;

	ProcessingConfigPresentation* mConfig;


public:
	ProcessingPresentation(const std::string& nameFromNetwork = "unnamed");
	virtual ~ProcessingPresentation();
	virtual void AttachTo(CLAMVM::ProcessingModel & );
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual const std::string & GetNameFromNetwork(){return mNameFromNetwork;}
	OutPortPresentation & GetOutPortPresentation( const std::string& );
	InPortPresentation & GetInPortPresentation( const std::string& );
	OutControlPresentation & GetOutControlPresentation( const std::string& );
	InControlPresentation & GetInControlPresentation( const std::string& );
	bool HasInPort( const std::string& name);
	bool HasOutPort( const std::string& name);

protected:
	virtual void OnNewConfig( CLAM::ProcessingConfig * );
	virtual void OnNewUpdateConfig( CLAM::ProcessingConfig * );
	virtual void OnNewObservedClassName( const std::string& ) = 0;
	virtual void OnNewInPort( CLAMVM::InPortAdapter* ) = 0;
	virtual void OnNewOutPort( CLAMVM::OutPortAdapter* ) = 0;
	virtual void OnNewInControl( CLAMVM::InControlAdapter* ) = 0;
	virtual void OnNewOutControl( CLAMVM::OutControlAdapter* ) = 0;

public:	//slots
	SigSlot::Signalv1< ProcessingPresentation* > RemoveProcessing;
	SigSlot::Slotv1<const std::string &> SetObservedClassName;
	SigSlot::Slotv1< CLAMVM::InPortAdapter* > SetInPort;
	SigSlot::Slotv1< CLAMVM::OutPortAdapter* > SetOutPort;	
	SigSlot::Slotv1< CLAMVM::InControlAdapter* > SetInControl;
	SigSlot::Slotv1< CLAMVM::OutControlAdapter* > SetOutControl;	

	SigSlot::Signalv1< CLAM::ProcessingConfig * > NewConfig;
	SigSlot::Signalv1< CLAM::ProcessingConfig * > UpdateConfig;
	SigSlot::Slotv1< CLAM::ProcessingConfig *> SetConfig;
	SigSlot::Slotv1< CLAM::ProcessingConfig *> SetConfigFromGUI;
};


} //namespace NetworkGUI

#endif //  __PROCESSINGPRESENTATION_HXX__
