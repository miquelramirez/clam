
#ifndef __PROCESSINGPRESENTATION_HXX__
#define __PROCESSINGPRESENTATION_HXX__


#include <vector>
#include <string>
#include <iostream>

#include "Presentation.hxx"
#include "Slotv1.hxx"

namespace CLAMVM
{
	class ProcessingModel;
	class InPortAdapter;
	class OutPortAdapter;
}

namespace NetworkGUI
{

	class InPortPresentation;
	class OutPortPresentation;

class ProcessingPresentation : public CLAMVM::Presentation
{
protected:
	std::list<InPortPresentation*> mInPortPresentations;
	std::list<OutPortPresentation*> mOutPortPresentations;
	typedef std::list<InPortPresentation*>::iterator InPortPresentationIterator;
	typedef std::list<OutPortPresentation*>::iterator OutPortPresentationIterator;
	std::string mName;
	std::string mNameFromNetwork;
	std::string mObservedClassName;
public:
	ProcessingPresentation(const std::string& nameFromNetwork = "unnamed");
	virtual ~ProcessingPresentation();
	virtual void AttachTo(CLAMVM::ProcessingModel & );
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual const std::string & GetName(){return mName;}
	virtual const std::string & GetNameFromNetwork(){return mNameFromNetwork;}
	OutPortPresentation & GetOutPortPresentation( const std::string& );
	InPortPresentation & GetInPortPresentation( const std::string& );

protected:
	virtual void OnNewName( const std::string& ) = 0;
	virtual void OnNewObservedClassName( const std::string& ) = 0;
	virtual void OnNewInPort( CLAMVM::InPortAdapter* ) = 0;
	virtual void OnNewOutPort( CLAMVM::OutPortAdapter* ) = 0;

public:	//slots
	SigSlot::Slotv1<const std::string &> SetName;
	SigSlot::Slotv1<const std::string &> SetObservedClassName;
	SigSlot::Slotv1< CLAMVM::InPortAdapter* > SetInPort;
	SigSlot::Slotv1< CLAMVM::OutPortAdapter* > SetOutPort;	
};

} //namespace NetworkGUI

#endif //  __PROCESSINGPRESENTATION_HXX__
