
#include "ProcessingController.hxx"
#include "InPortAdapter.hxx"
#include "OutPortAdapter.hxx"
#include "Processing.hxx"

#include <vector>

namespace CLAMVM
{

ProcessingController::ProcessingController()
	: mObserved(0),
	  mConfig(0)
{
}

ProcessingController::~ProcessingController()
{
	if (mConfig)
		delete mConfig;

	InPortAdapterIterator itin;
	for ( itin=mInPortAdapters.begin(); itin!=mInPortAdapters.end(); itin++)
		delete *itin;
	OutPortAdapterIterator itout;
	for ( itout=mOutPortAdapters.begin(); itout!=mOutPortAdapters.end(); itout++)
		delete *itout;
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

	AcquireConfig.Emit( mConfig );
	CLAM::Processing* proc = (CLAM::Processing*) mObserved;	
	CLAM::Processing::ConstInPortIterator itin;
	for (itin = proc->GetInPorts().Begin(); 
	     itin != proc->GetInPorts().End(); 
	     itin++)
	{
		InPortAdapter* adapter = new InPortAdapter;
		CLAM::InPort* inport = *itin;
		adapter->BindTo(*inport);
		mInPortAdapters.push_back(adapter);
		AcquireInPort.Emit(adapter);
	}
	
	CLAM::Processing::ConstOutPortIterator itout;
	for (itout = proc->GetOutPorts().Begin(); 
		     itout != proc->GetOutPorts().End(); 
	     itout++)
	{	
		OutPortAdapter* adapter = new OutPortAdapter;
		CLAM::OutPort* outport = *itout;
		adapter->BindTo(*outport);
		mOutPortAdapters.push_back(adapter);
		AcquireOutPort.Emit(adapter);
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
	return true;
}

} //namespace CLAMVM
