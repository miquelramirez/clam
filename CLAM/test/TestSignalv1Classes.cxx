#include "TestSignalv1Classes.hxx"
#include <iostream>

namespace CLAMTest
{

		// Signaler class

		void Signaler::Simulate()
		{
			mNewInteger.Emit( 33 );
		}

		Signalv1<int>& Signaler::GetNewIntegerSignal()
		{
			return mNewInteger;
		}

// Slotted class

		void Slotted::sReact( int value )
		{
			smNotified++;
			std::cout << "Value received " << value << std::endl;
		}

		void Slotted::PureVirtualReact( int value )
		{
			mPureVirtualNotified++;
			std::cout << "Value received" << value << std::endl;
		}

		void Slotted::React( int value )
		{
			mNotified++;
			std::cout << "Value received " << value << std::endl;
		}

		void Slotted::LinkWith( Signaler& sig )
		{
			sig.GetNewIntegerSignal().Connect( mSignalSlot );
			sig.GetNewIntegerSignal().Connect( mStaticSignalSlot );
			sig.GetNewIntegerSignal().Connect( mPureVirtualSignalSlot );
		}

		bool Slotted::WasCorrectlyNotified()
		{
			return (mNotified==1) 
					&& (smNotified==1) 
					&& (mPureVirtualNotified==1);
		}

		int Slotted::smNotified = 0;

// SlottedMultipleSignals class

		bool SlottedMultipleSignals::WasCorrectlyNotified()
		{
			return ( mNotified == mNumOfExpectedCalls )
				&& ( smNotified == mNumOfExpectedCalls )
				&& ( mPureVirtualNotified == mNumOfExpectedCalls );
		}


		void SlottedMultipleSignals::LinkWith( tSignalerVector& signalers )
		{
			smNotified = 0;
			tSignalerIterator i = signalers.begin();
			tSignalerIterator end = signalers.end();

			mNumOfExpectedCalls = signalers.size();

			while( i != end )
			{
				i->GetNewIntegerSignal().Connect( mSignalSlot );
				i->GetNewIntegerSignal().Connect( mStaticSignalSlot );
				i->GetNewIntegerSignal().Connect( mPureVirtualSignalSlot );

				i++;
			}

		}

}

