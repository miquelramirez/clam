#include "Fundamental2Control.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "Fundamental2Control",
		"category", "Controls",
		"description", "Fundamental2Control",
		0
	};
	static FactoryRegistrator<ProcessingFactory, Fundamental2Control> reg = metadata;
}

bool Fundamental2Control::Do( const Fundamental& inFund ) 
{	
	mFreqControlOut.SendControl( inFund.GetFreq(0) );
	return true;
}


}

