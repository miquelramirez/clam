
#include "ProcessingPresentation.hxx"
#include "InPortPresentation.hxx"
#include "OutPortPresentation.hxx"
#include "InControlPresentation.hxx"
#include "OutControlPresentation.hxx"
#include "InPortAdapter.hxx"
#include "OutPortAdapter.hxx"
#include "InControlAdapter.hxx"
#include "OutControlAdapter.hxx"
#include "ProcessingModel.hxx"
#include "ProcessingConfig.hxx"

namespace NetworkGUI
{

ProcessingPresentation::ProcessingPresentation(const std::string& nameFromNetwork)
	: mNameFromNetwork(nameFromNetwork)
{
	SetConfig.Wrap( this, &ProcessingPresentation::OnNewConfig );
	SetInPort.Wrap( this, &ProcessingPresentation::OnNewInPort );
	SetOutPort.Wrap( this, &ProcessingPresentation::OnNewOutPort );
	SetInControl.Wrap( this, &ProcessingPresentation::OnNewInControl );
	SetOutControl.Wrap( this, &ProcessingPresentation::OnNewOutControl );

	SetObservedClassName.Wrap( this, &ProcessingPresentation::OnNewObservedClassName );
}

void ProcessingPresentation::OnNewConfig( CLAM::ProcessingConfig * cfg)
{
	mConfig = cfg;
}

ProcessingPresentation::~ProcessingPresentation()
{
	InPortPresentationIterator itInPort;
	for ( itInPort=mInPortPresentations.begin(); itInPort!=mInPortPresentations.end(); itInPort++)
		delete *itInPort;
	OutPortPresentationIterator itOutPort;
	for ( itOutPort=mOutPortPresentations.begin(); itOutPort!=mOutPortPresentations.end(); itOutPort++)
		delete *itOutPort;
	InControlPresentationIterator itInControl;
	for ( itInControl=mInControlPresentations.begin(); itInControl!=mInControlPresentations.end(); itInControl++)
		delete *itInControl;
	OutControlPresentationIterator itOutControl;
	for ( itOutControl=mOutControlPresentations.begin(); itOutControl!=mOutControlPresentations.end(); itOutControl++)
		delete *itOutControl;

}

void ProcessingPresentation::AttachTo(CLAMVM::ProcessingModel & m)
{
	m.AcquireClassName.Connect(SetObservedClassName);
	m.AcquireInPort.Connect(SetInPort);
	m.AcquireOutPort.Connect(SetOutPort);
	m.AcquireInControl.Connect(SetInControl);
	m.AcquireOutControl.Connect(SetOutControl);
	m.AcquireConfig.Connect(SetConfig);
}

OutPortPresentation & ProcessingPresentation::GetOutPortPresentation( const std::string& name)
{
	OutPortPresentationIterator itout;
	for ( itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end(); itout++)
		if((*itout)->GetName() == name)
			return **itout;
}

InPortPresentation & ProcessingPresentation::GetInPortPresentation( const std::string& name)
{
	InPortPresentationIterator itin;
	for ( itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end(); itin++)
		if((*itin)->GetName() == name)
			return **itin;
}

OutControlPresentation & ProcessingPresentation::GetOutControlPresentation( const std::string& name)
{
	OutControlPresentationIterator itout;
	for ( itout=mOutControlPresentations.begin(); itout!=mOutControlPresentations.end(); itout++)
		if((*itout)->GetName() == name)
			return **itout;
}

InControlPresentation & ProcessingPresentation::GetInControlPresentation( const std::string& name)
{
	InControlPresentationIterator itin;
	for ( itin=mInControlPresentations.begin(); itin!=mInControlPresentations.end(); itin++)
		if((*itin)->GetName() == name)
			return **itin;
}

} //namespace NetworkGUI
