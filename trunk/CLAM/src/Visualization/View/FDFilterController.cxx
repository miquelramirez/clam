#include "FDFilterController.hxx"
#include "FDFilterGen.hxx"


namespace CLAMVM
{
	FDFilterController::FDFilterController()
	{
	}

	FDFilterController::~FDFilterController()
	{
	}

	bool FDFilterController::BindTo( Processing& proc )
	{
		ProcessingController::BindTo( proc );

		mObserved = dynamic_cast< FDFilterGen* >( &proc );

		CLAM_ASSERT( mObserved!=NULL, "Error: Trying to bind to a Processing that is not a FDFilterGen" );

		return true;

	}

	bool FDFilterController::Publish()
	{
		// Control value ranges
		// Configuration retrieval
		const CLAM::FDFilterGenConfig& cfg = dynamic_cast< const CLAM::FDFilterGenConfig& >( mObserved->GetConfig() );
		ControlAdapter* hCutoffAdapt = static_cast<ControlAdapter*>( mChildren["High Cutoff Frequency"]);
		ControlAdapter* lCutoffAdapt = static_cast<ControlAdapter*>( mChildren["Low Cutoff Frequency"]);
		ControlAdapter* gAdapt = static_cast<ControlAdapter*>( mChildren["Gain"]);

		CLAM::TData srange = cfg.GetSpectralRange();
		hCutoffAdapt->SetValueRange( 0, srange  );
		lCutoffAdapt->SetValueRange( 0, srange );
		gAdapt->SetValueRange( -70, 0 );

		ProcessingController::Publish();

		return true;		
	}
}
