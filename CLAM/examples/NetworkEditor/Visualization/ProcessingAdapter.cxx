
#include "ProcessingAdapter.hxx"
#include "Processing.hxx"
#include "InPortAdapter.hxx"
#include "OutPortAdapter.hxx"

#include <vector>
#include <iostream>


namespace CLAMVM
{

ProcessingAdapter::ProcessingAdapter()
	: mObserved (0)
{
}

ProcessingAdapter::~ProcessingAdapter()
{
	InPortAdapterIterator itin;
	for ( itin=mInPortAdapters.begin(); itin!=mInPortAdapters.end(); itin++)
		delete *itin;
	OutPortAdapterIterator itout;
	for ( itout=mOutPortAdapters.begin(); itout!=mOutPortAdapters.end(); itout++)
		delete *itout;
}
	

bool ProcessingAdapter::Publish()
{
	if ( !mObserved )  // there is no object being observed
		return false;
	
	AcquireName.Emit(mObserved->GetName());
	AcquireClassName.Emit(mObserved->GetClassName());
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

bool ProcessingAdapter::BindTo( const CLAM::Processing& obj )
{
	mObserved = dynamic_cast< const CLAM::Processing* > (&obj);
	
	if ( !mObserved )
	{ 
		return false;
	}
	return true;
}

} //namespace CLAMVM
