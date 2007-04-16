#ifndef __TESTSIGNALV1CLASSES__
#define __TESTSIGNALV1CLASSES__

#include "Slotv1.hxx"
#include "Signalv1.hxx"
#include <vector>

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

#endif // TestSignalv1Classes.hxx

