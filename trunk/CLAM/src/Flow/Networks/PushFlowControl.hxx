
#ifndef  _PushFlowControl_hxx_
#define  _PushFlowControl_hxx_

#include "FlowControl.hxx"
#include <list>

namespace CLAM
{

class Processing;

class PushFlowControl : public FlowControl
{
public:
	PushFlowControl( int frameSize = 0 );
	virtual ~PushFlowControl(){}

	void ProcessingAddedToNetwork( Processing & added );
	void DoProcessings();

private:
	void AddNewPossibleProcessingsToDo( Processing * father, 
					    std::list<Processing*> &,
					    std::list<Processing*> & );
	bool AreAllProducersExecuted( Processing * aProcessing, std::list<Processing*> & executedList );
	std::list< Processing* > _generators;


};

} // namespace CLAM

#endif //  _PushFlowControl_hxx_
