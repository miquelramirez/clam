#include "Fl_SMS_BPF_Editor.hxx"
#include "Fl_Envelope_Scroll.H"
#include <FL/Fl_Check_Button.H>
#include "CLAM_Math.hxx"
#include "BPF.hxx"

namespace CLAMVM
{
	Fl_SMS_BPF_Editor::Fl_SMS_BPF_Editor( int X, int Y, int W, int H )
		: Fl_Group( X, Y, W, H), mpFunctionEditor( NULL )
	{
		mpFunctionEditor = new Fl_Envelope_Scroll( X+5, Y+5, W-10, H-30 );
		mpFunctionEditor->envelope->grid( 0.1f, 0.1f );
		mpFunctionEditor->control->hvalue( 0.0, 1.0, 0.0, 1.0 );
		mpFunctionEditor->control->vvalue( 0.0, 1.0, 0.0, 1.0 );
		mpFunctionEditor->envelope->margin( 20, 10, 5, 5 );
		mpFunctionEditor->end();

		mpSnapToGridBtn = new Fl_Check_Button( X+10, H-15, W-40, 15 );
		mpSnapToGridBtn->label( "Snap points to grid" );
		mpSnapToGridBtn->labelsize( 10 );
		//mpSnapToGridBtn->align( FL_ALIGN_RIGHT );
		mpSnapToGridBtn->callback( (Fl_Callback*)cbSnapToGrid, this );

		end();
		resizable( mpFunctionEditor );

		NewPoint.Wrap( this, &Fl_SMS_BPF_Editor::OnNewPoint );
		PointRemoved.Wrap( this, &Fl_SMS_BPF_Editor::OnPointRemoved );
		PointMoved.Wrap( this, &Fl_SMS_BPF_Editor::OnPointMoved );

		mpFunctionEditor->envelope->PointAdded.Connect( NewPoint );
		mpFunctionEditor->envelope->PointMoved.Connect( PointMoved );
		mpFunctionEditor->envelope->PointRemoved.Connect( PointRemoved );
	}

	
	void Fl_SMS_BPF_Editor::OnNewPoint( double x, double y )
	{
		PointsChanged.Emit();
	}

	void Fl_SMS_BPF_Editor::OnPointRemoved()
	{
		PointsChanged.Emit();
	}
	
	void Fl_SMS_BPF_Editor::OnPointMoved()
	{
		PointsChanged.Emit();
	}

	void Fl_SMS_BPF_Editor::SetHorizontalRange( double xmin, double xmax )
	{
		mpFunctionEditor->control->hvalue( xmin, fabs(xmax-xmin), xmin, fabs(xmax-xmin) );
	}

	void Fl_SMS_BPF_Editor::SetVerticalRange( double ymin, double ymax )
	{
		mpFunctionEditor->control->vvalue( ymax, fabs( ymax-ymin), ymin, fabs(ymax-ymin) );
	}

	void Fl_SMS_BPF_Editor::SetGridWidth( double xwidth, double ywidth )
	{
		mpFunctionEditor->envelope->grid( xwidth, ywidth );
	}

	void Fl_SMS_BPF_Editor::cbSnapToGrid( Fl_Check_Button* btn, void* data )
	{
		Fl_SMS_BPF_Editor* pThis = (Fl_SMS_BPF_Editor*)data;

		if ( btn->value() )
			pThis->mpFunctionEditor->envelope->snap( SNAPBOTH );
		else
			pThis->mpFunctionEditor->envelope->snap( 0 );

	}

	Fl_SMS_BPF_Editor::~Fl_SMS_BPF_Editor()
	{
	}

	void Fl_SMS_BPF_Editor::InitPoints( double yvalue )
	{
		mpFunctionEditor->envelope->add_point( 0.0, yvalue );
		mpFunctionEditor->envelope->add_point( 1.0, yvalue );
	}

	void Fl_SMS_BPF_Editor::InitPoints( const CLAM::BPF& originalBPF )
	{
		for ( int i = 0; i < originalBPF.Size(); i++ )
			mpFunctionEditor->envelope->add_point( originalBPF.GetXValue( i ), 
							      originalBPF.GetValueFromIndex( i ) );
		
	}

	void Fl_SMS_BPF_Editor::Clear()
	{
		mpFunctionEditor->envelope->clear();
	}

	void Fl_SMS_BPF_Editor::InsertPointsIntoBPF( CLAM::BPF& editedBPF )
	{

		
		while( editedBPF.Size() )
			editedBPF.DeleteIndex(0);
		
		
		int i = 0;

		for ( int i = 0; i < mpFunctionEditor->envelope->points(); i++ )
		{
			FLPOINT& p = mpFunctionEditor->envelope->point(i);
			editedBPF.Insert( p.x, p.y );
		}

		
	}
}
