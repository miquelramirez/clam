
#include "ProcessingPresentation.hxx"
#include "InPortPresentation.hxx"
#include "OutPortPresentation.hxx"
#include "InPortAdapter.hxx"
#include "OutPortAdapter.hxx"
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
	SetObservedClassName.Wrap( this, &ProcessingPresentation::OnNewObservedClassName );
}

void ProcessingPresentation::OnNewConfig( CLAM::ProcessingConfig * cfg)
{
	mConfig = cfg;
}

ProcessingPresentation::~ProcessingPresentation()
{
	InPortPresentationIterator itin;
	for ( itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end(); itin++)
		delete *itin;
	OutPortPresentationIterator itout;
	for ( itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end(); itout++)
		delete *itout;
}

void ProcessingPresentation::AttachTo(CLAMVM::ProcessingModel & m)
{
	m.AcquireClassName.Connect(SetObservedClassName);
	m.AcquireInPort.Connect(SetInPort);
	m.AcquireOutPort.Connect(SetOutPort);
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


} //namespace NetworkGUI
