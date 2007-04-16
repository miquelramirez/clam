#include "FundamentalAdapterTestHelper.hxx"


namespace CLAMVM
{
	FundFreqPresStub::FundFreqPresStub()
		: mNotified( false )
	{
		NewTrajectory.Wrap( this, &FundFreqPresStub::OnNewTrajectory );
		NewTimeSpan.Wrap( this, &FundFreqPresStub::OnNewTimeSpan );
	}

	FundFreqPresStub::~FundFreqPresStub()
	{
	}

	void FundFreqPresStub::OnNewTrajectory( const CLAM::Array<TimeFreqMagPoint>& newObj, CLAM::TData )
	{
		AcknowledgeNotification();	
	}

	void FundFreqPresStub::OnNewTimeSpan( CLAM::TTime begin, CLAM::TTime end )
	{
	}

}

