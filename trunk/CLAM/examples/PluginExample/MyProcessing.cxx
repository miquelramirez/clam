#include "MyProcessing.hxx"
#include <CLAM/ProcessingFactory.hxx>

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MyProcessing>
	myProcessingRegistrator("MyProcessing");

