/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ProcessingPresentation.hxx"
#include "ProcessingConfig.hxx"
#include "ConnectionPointPresentation.hxx"
#include "Factory.hxx"

namespace NetworkGUI
{

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;

ProcessingPresentation::ProcessingPresentation(const std::string& name )
	: mName(name),
	  mConfig(0)
{
	SlotConfigureProcessing.Wrap( this, &ProcessingPresentation::ConfigureProcessing );
	SlotChangeProcessingPresentationName.Wrap( this, &ProcessingPresentation::ChangeProcessingPresentationName );
	SlotChangeState.Wrap( this, &ProcessingPresentation::ChangeState );
}

void ProcessingPresentation::SetConfig( const CLAM::ProcessingConfig & cfg)
{
	if (!mConfig)
	{
		ProcessingConfigPresentationFactory & factory =  ProcessingConfigPresentationFactory::GetInstance();
		mConfig = factory.Create(cfg.GetClassName());
		mConfig->SignalConfigureProcessing.Connect( SlotConfigureProcessing );
	}
	mConfig->SetConfig( cfg );
}

void ProcessingPresentation::ConfigureProcessing( const CLAM::ProcessingConfig & cfg)
{
	SignalConfigureProcessing.Emit( cfg );
}

void ProcessingPresentation::ChangeProcessingPresentationName( const std::string & name )
{
	mName = name;
}

void ProcessingPresentation::ChangeState( CLAMVM::ProcessingController::ProcessingExecState state, const std::string & status )
{
	mProcessingState = state;
	mProcessingStatus = status;
	UpdatePresentation();
}


ProcessingPresentation::~ProcessingPresentation()
{
	ConnectionPointPresentationsList::iterator it;
	for ( it=mInPortPresentations.begin(); it!=mInPortPresentations.end(); it++)
		delete *it;
	for ( it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end(); it++)
		delete *it;
	for ( it=mInControlPresentations.begin(); it!=mInControlPresentations.end(); it++)
		delete *it;
	for ( it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end(); it++)
		delete *it;

	mInPortPresentations.clear();
	mOutPortPresentations.clear();
	mInControlPresentations.clear();
	mOutControlPresentations.clear();

	if (mConfig)
	{
		mConfig->Hide();
		delete mConfig;
	}
}

void ProcessingPresentation::AttachTo(CLAMVM::ProcessingController & controller)
{
	CLAMVM::ProcessingController::NamesList::iterator it;
	for( it=controller.BeginInPortNames();it!=controller.EndInPortNames();it++)
		SetInPort(*it);
	
	for( it=controller.BeginOutPortNames();it!=controller.EndOutPortNames();it++)
		SetOutPort(*it);
	
	for( it=controller.BeginInControlNames();it!=controller.EndInControlNames();it++)
		SetInControl(*it);
	
	for( it=controller.BeginOutControlNames();it!=controller.EndOutControlNames();it++)
		SetOutControl(*it);

	SetObservedClassName( controller.GetObservedClassName() );
	SetConfig( controller.GetObservedConfig() );	
	
	SignalConfigureProcessing.Connect( controller.SlotConfigureProcessing );
	SignalProcessingNameChanged.Connect( controller.SlotProcessingNameChanged );
	SignalSendOutControlValue.Connect( controller.SlotSendOutControlValue );
	controller.SignalChangeProcessingPresentationName.Connect( SlotChangeProcessingPresentationName );
	controller.SignalChangeState.Connect( SlotChangeState );

	ChangeState( controller.GetProcessingExecState(), controller.GetProcessingStatus() );
}

ConnectionPointPresentation & ProcessingPresentation::GetOutPortPresentation( const std::string& name)
{
	ConnectionPointPresentationsList::iterator it;
	for ( it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end(); it++)
		if((*it)->GetName() == name)
			return **it;

	return *(ConnectionPointPresentation*)NULL;
}

ConnectionPointPresentation & ProcessingPresentation::GetInPortPresentation( const std::string& name)
{
	ConnectionPointPresentationsList::iterator it; 
	for ( it=mInPortPresentations.begin(); it!=mInPortPresentations.end(); it++)
		if((*it)->GetName() == name)
			return **it;	
	return *(ConnectionPointPresentation*)NULL;
}

ConnectionPointPresentation & ProcessingPresentation::GetOutControlPresentation( const std::string& name)
{
	ConnectionPointPresentationsList::iterator it;
	for ( it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end(); it++)
		if((*it)->GetName() == name)
			return **it;
	return *(ConnectionPointPresentation*)NULL;
}

ConnectionPointPresentation & ProcessingPresentation::GetInControlPresentation( const std::string& name)
{
	ConnectionPointPresentationsList::iterator it;
	for ( it=mInControlPresentations.begin(); it!=mInControlPresentations.end(); it++)
		if((*it)->GetName() == name)
			return **it;
	return *(ConnectionPointPresentation*)NULL;
}

bool ProcessingPresentation::HasInPort( const std::string& name)
{
	ConnectionPointPresentationsList::iterator itin;
	for ( itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end(); itin++)
		if((*itin)->GetName() == name)
			return true;
	return false;
}

bool ProcessingPresentation::HasOutPort( const std::string& name)
{
	ConnectionPointPresentationsList::iterator itout;
	for ( itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end(); itout++)
		if((*itout)->GetName() == name)
			return true;
	return false;	
}

void ProcessingPresentation::UpdateListOfPortsAndControls( CLAMVM::ProcessingController & controller )
{
	Hide();

	// delete all ports and controls of the processing
	ConnectionPointPresentationsList::iterator itc;
	for(itc=mInPortPresentations.begin(); itc!=mInPortPresentations.end(); itc++)
		delete (*itc);
	for(itc=mOutPortPresentations.begin(); itc!=mOutPortPresentations.end(); itc++)
		delete (*itc);
	for(itc=mInControlPresentations.begin(); itc!=mInControlPresentations.end(); itc++)
		delete (*itc);
	for(itc=mOutControlPresentations.begin(); itc!=mOutControlPresentations.end(); itc++)
		delete (*itc);

	mInPortPresentations.clear();
	mOutPortPresentations.clear();
	mInControlPresentations.clear();
	mOutControlPresentations.clear();

 
// create all ports and controls of processing
	CLAMVM::ProcessingController::NamesList::const_iterator it;
	for( it=controller.BeginInPortNames();it!=controller.EndInPortNames();it++)
		SetInPort(*it);
	
	for( it=controller.BeginOutPortNames();it!=controller.EndOutPortNames();it++)
		SetOutPort(*it);
	
	for( it=controller.BeginInControlNames();it!=controller.EndInControlNames();it++)
		SetInControl(*it);
	
	for( it=controller.BeginOutControlNames();it!=controller.EndOutControlNames();it++)
		SetOutControl(*it);

	Show(); 	
}

} //namespace NetworkGUI
