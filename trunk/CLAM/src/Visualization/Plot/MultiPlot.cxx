#include "MultiPlot.hxx"
#include "Fl_MultiPlot.hxx"
#include "SequenceRenderingManager.hxx"

namespace CLAMVM
{

	MultiPlot::MultiPlot( )
	{
		CreateWidget();
		RegisterAnonymously();
	}

	MultiPlot::MultiPlot( std::string name )
	{
		CreateWidget();
		SetName( name );
		RegisterAs( name );
	}


	MultiPlot::~MultiPlot()
	{
		RemoveAllData();
	}

	void MultiPlot::CreateWidget()
	{
		SetWidget( new Fl_MultiPlot() );
		GetWidget()->label( GetName().c_str() );
	}

	void MultiPlot::SetColor( std::string key, Color c )
	{
		if ( !ExistsRenderer( key ) )
		{
			CreateNewRenderer( key );
			UpdateWidgetRenderers();
		}
		
		mRenderers[ key ]->SetLineColor( c );
	}

	bool MultiPlot::ExistsRenderer( const std::string& key )
	{
		return mRenderers.find( key ) != mRenderers.end();
	}

	void MultiPlot::CreateNewRenderer( const std::string& key )
	{
		mRenderers[key] = new SequenceRenderingManager;
	}

	void MultiPlot::UpdateWidgetRenderers()
	{
		Fl_MultiPlot* pWidget = static_cast< Fl_MultiPlot* >( GetWidget() );

		for ( RendererTable::iterator i = mRenderers.begin();
		      i != mRenderers.end();
		      i++ )
			pWidget->AddRenderer( i->second );
	}

	void MultiPlot::AddData( std::string key, const CLAM::Array<CLAM::TData>& array, double xmin, double xmax )
	{
		if ( !ExistsRenderer( key ) )
		{
			CreateNewRenderer( key );
			UpdateWidgetRenderers();
		}

		mRenderers[key]->SetDataXRange( xmin, xmax );

		GetWidget()->SetXRange( MinimumXOfAll(), MaximumXOfAll() );

		mRenderers[key]->CacheData( array );
	}

	double MultiPlot::MinimumXOfAll()
	{
		double minX = 0.0;
		
		RendererTable::iterator i = mRenderers.begin();

		minX = (i->second)->GetDataBounds().mLeft;
		i++;

		for ( ; i!= mRenderers.end(); i++ )
			if ( (i->second)->GetDataBounds().mLeft < minX )
				minX = (i->second)->GetDataBounds().mLeft;

		return minX;
	}

	double MultiPlot::MaximumXOfAll()
	{
		double maxX = 0.0;
		
		RendererTable::iterator i = mRenderers.begin();

		maxX = (i->second)->GetDataBounds().mRight;
		i++;

		for ( ; i!= mRenderers.end(); i++ )
			if ( (i->second)->GetDataBounds().mRight > maxX )
				maxX = (i->second)->GetDataBounds().mRight;

		return maxX;
		
	}

	void MultiPlot::AddData( std::string key, const CLAM::BPF& data, int samples )
	{
		if ( !ExistsRenderer( key ) )
		{
			CreateNewRenderer( key );
			UpdateWidgetRenderers();
		}
		CLAM::DataArray sampledBPF;

		CLAM::TData start = data.GetXValue( 0 );
		CLAM::TData end = data.GetXValue( data.Size() - 1 );

		CLAM::TData dx = fabs( end - start ) / TData( samples );

		sampledBPF.Resize( samples+1 );
		sampledBPF.SetSize( samples+1 );

		CLAM::TData x = start;

		for ( int i = 0; i < sampledBPF.Size(); i++ )
		{
			sampledBPF[i] = data.GetValue( x );
			x += dx;
		}

		mRenderers[key]->CacheData( sampledBPF );
		mRenderers[key]->SetDataXRange( start, end );
		GetWidget()->SetXRange( MinimumXOfAll(), MaximumXOfAll() );

	}

	void MultiPlot::RemoveData( std::string key )
	{
		RendererTable::iterator i = mRenderers.find( key );

		if ( i == mRenderers.end() )
			return;

		
		Fl_MultiPlot* pWidget = static_cast< Fl_MultiPlot* >( GetWidget() );

		pWidget->RemoveRenderer( mRenderers[key] );
		delete mRenderers[key];
		mRenderers.erase( i );
	}

	void MultiPlot::RemoveAllData( )
	{
		for ( RendererTable::iterator i = mRenderers.begin();
		      i != mRenderers.end();
		      i++ )
			delete i->second;

		mRenderers.clear();
		Fl_MultiPlot* pWidget = static_cast< Fl_MultiPlot* >( GetWidget() );

		pWidget->ClearRenderers();
	}



}
