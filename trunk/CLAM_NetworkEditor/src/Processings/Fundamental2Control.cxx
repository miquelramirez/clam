#include "Fundamental2Control.hxx"
#include "Factory.hxx"

namespace CLAM
{

bool Fundamental2Control::Do( const Fundamental& inFund ) 
{	
	mFreqControlOut.SendControl( inFund.GetFreq(0) );
	return true;
}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::Fundamental2Control> regtFundamental2Control( "Fundamental2Control" );

}
