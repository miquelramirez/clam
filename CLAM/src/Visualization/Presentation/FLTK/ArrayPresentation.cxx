#include "ArrayPresentation.hxx"
#include "MinMaxFinder.hxx"
#include "Assert.hxx"
using namespace CLAMGUI;

void ArrayPresentation::Show()
{
	mMainWidget->show();
}

void ArrayPresentation::UpdateData( const DataArray& array )
{
	// We search for the maximum and the minimum in the array

		CLAM_ASSERT( array.GetPtr(), " There's no data inside the array " );

	std::pair< TData, TData > min_max;

	minmaxelements( array.GetPtr(), array.GetPtr() + array.Size(), min_max );

	mMetrix.top = min_max.second * TData( 1.25 );
	mMetrix.top_es = true;
	mMetrix.bottom = min_max.first * TData( 1.25 );
	mMetrix.bottom_es = true;

	mMetrix.isIntX = true;
	mMetrix.isIntY = true;

	mRenderer->DefineViewport( array, mMetrix );

	mContainer->SetHorRange( mMetrix.left, mMetrix.right );
	mContainer->SetVerRange( mMetrix.top , -fabs( mMetrix.top - mMetrix.bottom ) );
	mContainer->mpHorRuler->mInteger = mMetrix.isIntX;
	mContainer->mpVerRuler->mInteger = false;

	mRenderer->CacheData( array );
}

void ArrayPresentation::Init( const Geometry& g, const char* label )
{
	mMainWidget = new Fl_Double_Window( g.GetX(), g.GetY(), g.GetW(), g.GetH(), label );
	mContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );

	mPort = new GLPort( Rect<int>( 0, 0, g.GetW(), g.GetH() ) );
	mRenderer = new GLArrayRenderer( 0, 200, 0 );

	mContainer->Add( mPort );
	mMainWidget->resizable( mContainer );
	mMainWidget->end();

	mPort->SetDrawingCallback( makeMemberFunctor0( *mRenderer, GLArrayRenderer, Draw) );

	mPort->Configure( new ArrayPresGLState, makeMemberFunctor0( *mRenderer, GLArrayRenderer, Draw ) );

}
