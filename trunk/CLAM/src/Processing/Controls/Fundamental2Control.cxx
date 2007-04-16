#include "Fundamental2Control.hxx"
#include "Factory.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<CLAM::Fundamental2Control> regtFundamental2Control( "Fundamental2Control" );
	}

bool Fundamental2Control::Do( const Fundamental& inFund ) 
{	
	mFreqControlOut.SendControl( inFund.GetFreq(0) );
	return true;
}


}

