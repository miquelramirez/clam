
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
#include "Factory.hxx"

#include <iostream>

namespace NetworkGUI
{

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;

ProcessingPresentation::ProcessingPresentation(const std::string& nameFromNetwork)
	: mNameFromNetwork(nameFromNetwork),
	  mConfig(0)
{
	SetConfig.Wrap( this, &ProcessingPresentation::OnNewConfig );
	SetConfigFromGUI.Wrap( this, &ProcessingPresentation::OnNewUpdateConfig );
	SetInPort.Wrap( this, &ProcessingPresentation::OnNewInPort );
	SetOutPort.Wrap( this, &ProcessingPresentation::OnNewOutPort );
	SetInControl.Wrap( this, &ProcessingPresentation::OnNewInControl );
	SetOutControl.Wrap( this, &ProcessingPresentation::OnNewOutControl );

	SetObservedClassName.Wrap( this, &ProcessingPresentation::OnNewObservedClassName );
}

void ProcessingPresentation::OnNewConfig( CLAM::ProcessingConfig * cfg)
{
	if (!mConfig)
	{
		ProcessingConfigPresentationFactory & factory =  ProcessingConfigPresentationFactory::GetInstance();
		mConfig = factory.Create(cfg->GetClassName());
		NewConfig.Connect( mConfig->SetConfig );
		mConfig->ApplyConfig.Connect( SetConfigFromGUI );

	}

	NewConfig.Emit( cfg );

}

void ProcessingPresentation::OnNewUpdateConfig( CLAM::ProcessingConfig * cfg)
{
	UpdateConfig.Emit( cfg );
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

	if (mConfig)
	{
		mConfig->Hide();
		delete mConfig;
	}
}

void ProcessingPresentation::AttachTo(CLAMVM::ProcessingModel & m)
{
	m.AcquireClassName.Connect(SetObservedClassName);
	m.AcquireInPort.Connect(SetInPort);
	m.AcquireOutPort.Connect(SetOutPort);
	m.AcquireInControl.Connect(SetInControl);
	m.AcquireOutControl.Connect(SetOutControl);
	m.AcquireConfig.Connect(SetConfig);
	UpdateConfig.Connect( m.SetNewConfig );
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

bool ProcessingPresentation::HasInPort( const std::string& name)
{
	InPortPresentationIterator itin;
	for ( itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end(); itin++)
		if((*itin)->GetName() == name)
			return true;
	return false;
}

bool ProcessingPresentation::HasOutPort( const std::string& name)
{
	OutPortPresentationIterator itout;
	for ( itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end(); itout++)
		if((*itout)->GetName() == name)
			return true;
	return false;	
}

} //namespace NetworkGUI
