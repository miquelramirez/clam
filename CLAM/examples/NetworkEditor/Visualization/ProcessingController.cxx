
#include "ProcessingController.hxx"
#include "InPortAdapter.hxx"
#include "OutPortAdapter.hxx"
#include "InControlAdapter.hxx"
#include "OutControlAdapter.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"

#include <vector>

#include <iostream>
namespace CLAMVM
{

ProcessingController::ProcessingController()
	: mObserved(0),
	  mConfig(0)
{
	SetNewConfig.Wrap( this, &ProcessingController::OnUpdateConfigFromGUI );
}

ProcessingController::~ProcessingController()
{
	if (mConfig)
		delete mConfig;

	InPortAdapterIterator itPortIn;
	for ( itPortIn=mInPortAdapters.begin(); itPortIn!=mInPortAdapters.end(); itPortIn++)
		delete *itPortIn;
	OutPortAdapterIterator itPortOut;
	for ( itPortOut=mOutPortAdapters.begin(); itPortOut!=mOutPortAdapters.end(); itPortOut++)
		delete *itPortOut;
	InControlAdapterIterator itCtrlIn;
	for ( itCtrlIn=mInControlAdapters.begin(); itCtrlIn!=mInControlAdapters.end(); itCtrlIn++)
		delete *itCtrlIn;
	OutControlAdapterIterator itCtrlOut;
	for ( itCtrlOut=mOutControlAdapters.begin(); itCtrlOut!=mOutControlAdapters.end(); itCtrlOut++)
		delete *itCtrlOut;

}

void ProcessingController::OnUpdateConfigFromGUI( CLAM::ProcessingConfig * cfg) 
{
	mConfig = (CLAM::ProcessingConfig*)cfg->DeepCopy();
//	if (mConfig)
//		delete mConfig;
//	mConfig = new CLAM::ProcessingConfig(*cfg);
	Update();
}
	

bool ProcessingController::Publish()
{
	if ( !mObserved )  // there is no object being observed
		return false;
	
	AcquireClassName.Emit( mObserved->GetClassName() );
	if (mConfig)
		delete mConfig;

	const CLAM::ProcessingConfig & conf( mObserved->GetConfig() );
	mConfig = (CLAM::ProcessingConfig*)conf.DeepCopy();
	std::cout << mConfig->GetClassName() << std::endl;
	AcquireConfig.Emit( mConfig );
	CLAM::Processing* proc = (CLAM::Processing*) mObserved;	
	CLAM::Processing::ConstInPortIterator itPortIn;
	for (itPortIn = proc->GetInPorts().Begin(); 
	     itPortIn != proc->GetInPorts().End(); 
	     itPortIn++)
	{
		InPortAdapter* adapter = new InPortAdapter;
		CLAM::InPort* inport = *itPortIn;
		adapter->BindTo(*inport);
		mInPortAdapters.push_back(adapter);
		AcquireInPort.Emit(adapter);
	}
	
	CLAM::Processing::ConstOutPortIterator itPortOut;
	for (itPortOut = proc->GetOutPorts().Begin(); 
	     itPortOut != proc->GetOutPorts().End(); 
	     itPortOut++)
	{	
		OutPortAdapter* adapter = new OutPortAdapter;
		CLAM::OutPort* outport = *itPortOut;
		adapter->BindTo(*outport);
		mOutPortAdapters.push_back(adapter);
		AcquireOutPort.Emit(adapter);
	}


	CLAM::Processing::ConstInControlIterator itCtrlIn;
	for (itCtrlIn = proc->GetInControls().Begin(); 
	     itCtrlIn != proc->GetInControls().End(); 
	     itCtrlIn++)
	{
		InControlAdapter* adapter = new InControlAdapter;
		CLAM::InControl* incontrol = *itCtrlIn;
		adapter->BindTo(*incontrol);
		mInControlAdapters.push_back(adapter);
		AcquireInControl.Emit(adapter);
	}
	
	CLAM::Processing::ConstOutControlIterator itCtrlOut;
	for (itCtrlOut = proc->GetOutControls().Begin(); 
	     itCtrlOut != proc->GetOutControls().End(); 
	     itCtrlOut++)
	{	
		OutControlAdapter* adapter = new OutControlAdapter;
		CLAM::OutControl* outcontrol = *itCtrlOut;
		adapter->BindTo(*outcontrol);
		mOutControlAdapters.push_back(adapter);
		AcquireOutControl.Emit(adapter);
	}

	return true;
}

bool ProcessingController::BindTo( CLAM::Processing& obj )
{
	mObserved = dynamic_cast< CLAM::Processing* > (&obj);
	
	if ( !mObserved )
	{ 
		return false;
	}
	return true;
}

bool ProcessingController::Update()
{
	mObserved->Configure( *mConfig );
	std::cout << "configuration updated" << std::endl;
	return true;
}

} //namespace CLAMVM
