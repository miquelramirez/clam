#ifndef __TESTSIGNALV1CLASSES__
#define __TESTSIGNALV1CLASSES__

#include "Signalv1.hxx"
#include "Slot.hxx"
#include <vector>

namespace CLAMTest
{

	using CLAMGUI::Signalv1;
	using CLAMGUI::Slot;

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
			}

			static void sReact( int value );

			void PureVirtualReact( int value );

			void React( int value );

			void LinkWith( Signaler& sig );

			virtual bool WasCorrectlyNotified();

	protected:

		int        mNotified;
		int        mPureVirtualNotified;
		static int smNotified;
		Slot       mSignalSlot;
		Slot       mStaticSignalSlot;
		Slot       mPureVirtualSignalSlot;


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

#endif // TestSignalv1Classes.hxx
