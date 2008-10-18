#include <iostream>
#include "Slotv1.hxx"
#include "Signalv1.hxx"
#include <vector>
#include <cstdlib>

using SigSlot::Slotv1;
using SigSlot::Signalv1;

namespace CLAMTest
{

	class Signaler
	{
	public:
			void Simulate();
			
			Signalv1<int>& GetNewIntegerSignal();

	private:

		Signalv1<int> mNewInteger;
	};
	
	class SlottedBase
	{
	public:
		virtual void PureVirtualReact( int value ) = 0;

		virtual bool WasCorrectlyNotified() = 0;


		virtual ~SlottedBase()
		{
		}
	};

	class Slotted
		: public SlottedBase
	{
	public:
		Slotted()
			: mNotified(0), mPureVirtualNotified(0)
		{
			mSignalSlot.Wrap(this, &Slotted::React);
			mStaticSignalSlot.Wrap(&Slotted::sReact);
			mPureVirtualSignalSlot.Wrap(this, &Slotted::PureVirtualReact);
		}

		static void sReact( int value );

		void PureVirtualReact( int value );

		void React( int value );

		void LinkWith( Signaler& sig );

		virtual bool WasCorrectlyNotified();

	protected:

		int         mNotified;
		int         mPureVirtualNotified;
		static int  smNotified;
		Slotv1<int> mSignalSlot;
		Slotv1<int> mStaticSignalSlot;
		Slotv1<int> mPureVirtualSignalSlot;


	};

	typedef std::vector<Signaler>           tSignalerVector;
	typedef std::vector<Signaler>::iterator tSignalerIterator;

	class SlottedMultipleSignals
		: public Slotted
	{
	public:

		SlottedMultipleSignals()
			: Slotted(), mNumOfExpectedCalls( 0 )
		{
		}

		virtual bool WasCorrectlyNotified();

		void LinkWith( tSignalerVector& multipleSignals );

	private:

		int        mNumOfExpectedCalls;

	};

		
}
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

			std::abort();
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


