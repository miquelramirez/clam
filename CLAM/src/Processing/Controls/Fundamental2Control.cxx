#include "Fundamental2Control.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, Fundamental2Control> regFundamental2Control("Fundamental2Control");
}

bool Fundamental2Control::Do( const Fundamental& inFund ) 
{	
	mFreqControlOut.SendControl( inFund.GetFreq(0) );
	return true;
}


}

