

#include "PushFlowControl.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"
#include "InPort.hxx"
#include "Node.hxx"
#include <iostream>

namespace CLAM
{

PushFlowControl::PushFlowControl( int frameSize )
	: FlowControl (frameSize)
{
}

void PushFlowControl::ProcessingAddedToNetwork( Processing & added )
{
	NetworkTopologyChanged();

	ConfigurePorts( added );
	if (added.GetInPorts().Size() == 0) // if it's a generator
		mGenerators.push_back( &added );
}

void PushFlowControl::DoProcessings()
{
	std::list< Processing* > toDo(mGenerators);
	std::list< Processing* > done;

	while (!toDo.empty())
	{
		// pop the next processing
		Processing * next = *(toDo.begin()); // the first
		toDo.pop_front();

		if(next->CanDoUsingPorts())
		{
			next->Do();
			done.push_back(next);
		}

		AddNewPossibleProcessingsToDo(next, toDo, done);
	}
}


void PushFlowControl::AddNewPossibleProcessingsToDo(
	Processing * producer, 
	std::list<Processing*> & toDo,
	std::list<Processing*> & executed )
{
	
	// for each out port of the processing already executed
	Processing::OutPortIterator itOutPort;		
	for (itOutPort=producer->GetOutPorts().Begin(); 
	     itOutPort!=producer->GetOutPorts().End(); 
	     itOutPort++)
	{
		if (!(*itOutPort)->GetNode())
			break;
		
		// for each processing connected as a consumer to the node
//		std::list<InPort*> consumers = (*itOutPort)->GetNode()->GetReaders();
		NodeBase::ReaderIterator consumers;
//		std::list< InPort* >::iterator itInPort;
		for (consumers=(*itOutPort)->GetNode()->BeginReaders(); 
		     consumers!=(*itOutPort)->GetNode()->EndReaders(); 
		     consumers++)
		{
			Processing * consumer = (Processing*)(*consumers)->GetProcessing();
			if (AreAllProducersExecuted( consumer, executed ))
				toDo.push_back( consumer );
		}
	}
}

bool PushFlowControl::AreAllProducersExecuted( Processing * son, std::list<Processing*> & done)
{
	Processing::InPortIterator itInPort;
	for (itInPort=son->GetInPorts().Begin(); 
	     itInPort!=son->GetInPorts().End(); 
	     itInPort++)
	{
		if (!(*itInPort)->GetNode())
			break;
		
		OutPort * oneWriter = (OutPort*)(*itInPort)->GetNode()->GetWriter();
		Processing * oneFather = (Processing*)oneWriter->GetProcessing();

		std::list< Processing* >::iterator itProcessingDone;
		bool isFatherDone = false;
		for (itProcessingDone=done.begin(); itProcessingDone!=done.end(); itProcessingDone++)
		{
				if ((*itProcessingDone) == oneFather)
					isFatherDone = true;
		}
		if (!isFatherDone)
			return false;
	}
	return true;
}

} // namespace CLAM


