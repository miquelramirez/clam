#ifndef __FUNDAMENTALADAPTER__
#define __FUNDAMENTALADAPTER__

#include "DataTypes.hxx"
#include "ModelAdapter.hxx"
#include "Array.hxx"
#include "TimeFreqMagPoint.hxx"
#include "Signalv2.hxx"

namespace CLAM
{
	class Segment;
}

namespace CLAMVM
{
	class FundamentalAdapter : public ModelAdapter
	{
	public:
		
		FundamentalAdapter();
		virtual ~FundamentalAdapter();

		virtual const char* GetClassName() const {
			return "FundamentalAdapter";
		}

		virtual bool Publish();

		virtual bool BindTo( const CLAM::Segment& adaptee );

		const CLAM::Segment* Adaptee() const	{
			return mAdaptedObject;
		}

		bool  HasCachedData() const {
			return mFundFreqTrajectory.Size() != 0;
		}

	protected: // methods
		
		bool ExtractTrajectory();

	public: // Signals and Slots
		
		SigSlot::Signalv2< const CLAM::Array<TimeFreqMagPoint>&, CLAM::TData  >  TrajectoryExtracted;
		SigSlot::Signalv2< CLAM::TTime , CLAM::TTime > TimeSpanChanged;

	protected:

		const CLAM::Segment*          mAdaptedObject;
		CLAM::Array<TimeFreqMagPoint> mFundFreqTrajectory;
	};
}

#endif // FundamentalAdapter.hxx
