#include "TestSignalv1Classes.hxx"
#include <iostream>

namespace CLAMTest
{

	void Check( SlottedBase& s )
	{
		if ( s.WasCorrectlyNotified() )
		{

			std::cout << "Test Passed" << std::endl;
		}
		else
		{
			std::cerr << "Test Failed" << std::endl;
			std::cerr << "Receiver was not notified" << std::endl;

			abort();
		}

	}

	void BasicTest()
	{
		Signaler emitter;
		Slotted  receiver;

		receiver.LinkWith( emitter );

		emitter.Simulate();

		Check( receiver );

		std::cout << "Basic Functionality test passed" << std::endl;
	}

	void MultipleSignalsConnectedTest()
	{
		tSignalerVector emitterVector;
		int k = 0;
		int signalCardinal = 10;

		for ( k = 0; k < signalCardinal; k++ )
		{
			emitterVector.push_back( Signaler() );
		}

		SlottedMultipleSignals receiver;

		receiver.LinkWith( emitterVector );

		for ( k = 0; k < signalCardinal; k++ )
		{
			emitterVector[k].Simulate();
		}

		Check( receiver );

		std::cout << "Multiple Signals connected to same slot test passed" << std::endl;
	}

}


using namespace CLAMTest;

	int main( int argc, char** argv )
	{

		BasicTest();
		MultipleSignalsConnectedTest();
		
		return 0;
	}


