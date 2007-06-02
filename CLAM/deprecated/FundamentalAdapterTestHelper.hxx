#ifndef __FUNDAMENTALADAPTERTESTHELPER__
#define __FUNDAMENTALADAPTERTESTHELPER__

#include "Presentation.hxx"
#include "Slotv2.hxx"
#include "Array.hxx"
#include "TimeFreqMagPoint.hxx"

namespace CLAMVM
{
	class FundFreqPresStub : public Presentation
	{
	public:
		FundFreqPresStub();
		
		virtual ~FundFreqPresStub();

		virtual void Show() { }
		virtual void Hide() { }
		
		SigSlot::Slotv2< const CLAM::Array<TimeFreqMagPoint>&, CLAM::TData > NewTrajectory;
		SigSlot::Slotv2< CLAM::TTime, CLAM::TTime >  NewTimeSpan;

		bool WasNotified() const {
			return mNotified;
		}


	protected:

		void AcknowledgeNotification() {
			mNotified = true;
		}


		void OnNewTrajectory( const CLAM::Array<TimeFreqMagPoint>& newObj, CLAM::TData );
		void OnNewTimeSpan( CLAM::TTime begin, CLAM::TTime end );

	private:
		bool mNotified;
		

	
	};

}


#endif // FundamentalAdapterTestHelper.hxx

