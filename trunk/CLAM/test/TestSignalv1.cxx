#include "Signalv1.hxx"
#include "Slot.hxx"
#include <iostream>

namespace CLAMTest
{
	using CLAMGUI::Signalv1;
	using CLAMGUI::Slot;

	class Signaler
	{
	public:
		void Simulate()
		{
			mNewInteger.Emit( 33 );
		}

		Signalv1<int>& GetNewIntegerSignal()
		{
			return mNewInteger;
		}

	private:

		Signalv1<int> mNewInteger;
	};
	
	class SlottedBase
	{
	public:
		virtual void PureVirtualReact( int value ) = 0;

		virtual ~SlottedBase()
		{
		}
	};

	class Slotted
		: public SlottedBase
	{
	public:

		static void sReact( int value )
		{
			smNotified = true;
			std::cout << "Value received " << value << std::endl;
		}

		void PureVirtualReact( int value )
		{
			mPureVirtualNotified = true;
			std::cout << "Value received" << value << std::endl;
		}

		void React( int value )
		{
			mNotified = true;
			std::cout << "Value received " << value << std::endl;
		}

		void LinkWith( Signaler& sig )
		{
			mSignalSlot = sig.GetNewIntegerSignal().Connect( this, &Slotted::React );
			mStaticSignalSlot = sig.GetNewIntegerSignal().Connect( &Slotted::sReact );
			mPureVirtualSignalSlot = sig.GetNewIntegerSignal().Connect( this, &Slotted::PureVirtualReact );
		}

		bool WasNotified()
		{
			return mNotified && smNotified && mPureVirtualNotified;
		}

	private:

		bool mNotified;
		bool mPureVirtualNotified;
		static bool smNotified;
		Slot mSignalSlot;
		Slot mStaticSignalSlot;
		Slot mPureVirtualSignalSlot;

	};

	bool Slotted::smNotified = false;

	void Check( Slotted& s )
	{
		if ( s.WasNotified() )
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

}


using namespace CLAMTest;

	int main( int argc, char** argv )
	{

		BasicTest();

		
		return 0;
	}


