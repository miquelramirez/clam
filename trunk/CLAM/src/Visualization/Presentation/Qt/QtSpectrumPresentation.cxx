#include "QtSpectrumPresentation.hxx"
#include "QtDataDisplay.hxx"
#include "QtGLPort.hxx"
using CLAM::DB;
using namespace CLAMGUI;

void QtSpectrumPresentation::PublishCallbacks()
{
	Aspect my_aspect;
		
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetMag"), &mGetMag );
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetPhase"), &mGetPhase );
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetSpectralRange"), &mGetRange );
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetSpectralResolution"), &mGetResolution );
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetScale"), &mGetScale );
	
	mLinkedView->Configure( my_aspect );
}

void QtSpectrumPresentation::Show()
{
	display->show();
}

void QtSpectrumPresentation::UpdateMagData( const DataArray& array )
{
	if ( !mSpectralMetrixLocked )
		{
			mRenderer->DefineViewport( array, mSpecPresMetrix );

			mPort->SetView( mSpecPresMetrix );

			mSpectralMetrixLocked = true;
		}

	mRenderer->CacheData( array );
}

void QtSpectrumPresentation::UpdatePhaseData( const DataArray& array )
{
	
}

void QtSpectrumPresentation::UpdateSpectralRangeData( TData value )
{
	if ( mRenderer->GetSamplingRate() != value )
		{
			mRenderer->SetSamplingRate( value );
			mSpectralMetrixLocked = false;
		}
}

void QtSpectrumPresentation::UpdateSpectralResolutionData( TData value )
{
	static TData last_value = 0;
	
	if ( last_value != value )
		{
			last_value = value;
			mSpectralMetrixLocked = false;
		}
}

void QtSpectrumPresentation::UpdateScale( unsigned int nbins, EScale scale )
{
	if ( scale.GetValue() == EScale::eLinear )
		{
			mSpecPresMetrix.SetTopEstimation( TData( nbins ) );
			mSpecPresMetrix.SetBottomEstimation( TData( 0 ) );
		}
	else if ( scale.GetValue() == EScale::eLog )
		{
			//	mSpecPresMetrix.SetTopEstimation( DB( TData(nbins), CLAM_DB_SCALING ) );
			mSpecPresMetrix.SetTopEstimation( 70 );
			mSpecPresMetrix.SetBottomEstimation( DB( TData(0), CLAM_DB_SCALING ) );
		}
	else
		CLAM_ASSERT( false, "Spectrum was crippled somehow since it hasn't provided a valid scale" );
}

void QtSpectrumPresentation::Init( const Geometry& g, const char* label )
{
	mRenderer = new GLLinearSpRenderer( 0, 200, 0 );
	
	display = new QtDataDisplay( NULL, "spectrum_display" );
	
	display->setCaption( label );

	mPort = new QtGLPort( display, "spectrum_renderer_host" );

	mPort->Configure( new SpecGLSetup, makeMemberFunctor0( *mRenderer, GLLinearSpRenderer, Draw ) );

	display->SetupContents( mPort );

	// Initializing the callback placeholders
	mGetMag = makeMemberFunctor1( const DataArray&, *this, QtSpectrumPresentation, UpdateMagData );
	mGetPhase = makeMemberFunctor1( const DataArray&, *this, QtSpectrumPresentation, UpdatePhaseData );
	mGetRange = makeMemberFunctor1( TData, *this, QtSpectrumPresentation, UpdateSpectralRangeData );
	mGetResolution = makeMemberFunctor1( TData, *this, QtSpectrumPresentation, UpdateSpectralResolutionData );
	mGetScale = makeMemberFunctor2( unsigned int, EScale, *this, QtSpectrumPresentation, UpdateScale );

}
