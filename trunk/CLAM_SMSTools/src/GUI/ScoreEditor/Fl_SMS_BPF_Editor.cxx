#include "Fl_SMS_BPF_Editor.hxx"
#include "Fl_Envelope_Scroll.H"
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Box.H>
#include "CLAM_Math.hxx"
#include "BPF.hxx"
#include "Assert.hxx"
#include "SMS_ScoreEditor_Helper.hxx"

namespace CLAMVM
{
	Fl_SMS_BPF_Editor::Fl_SMS_BPF_Editor( int X, int Y, int W, int H )
		: Fl_Group( X, Y, W, H), mpFunctionEditor( NULL ), mXAxisLabel( "No label set" ),
		  mYAxisLabel( "No label set" ), mpXAxisLabelBox( NULL ), mpYAxisLabelBox( NULL )
	{
		CreateContents();
		end();
		ApplyLayoutOnContents( X, Y, W, H );

		NewPoint.Wrap( this, &Fl_SMS_BPF_Editor::OnNewPoint );
		PointRemoved.Wrap( this, &Fl_SMS_BPF_Editor::OnPointRemoved );
		PointMoved.Wrap( this, &Fl_SMS_BPF_Editor::OnPointMoved );

		FunctionEditor().envelope->PointAdded.Connect( NewPoint );
		FunctionEditor().envelope->PointMoved.Connect( PointMoved );
		FunctionEditor().envelope->PointRemoved.Connect( PointRemoved );
	}

	void Fl_SMS_BPF_Editor::CreateContents()
	{
		mpFunctionEditor = new Fl_Envelope_Scroll( 0,0,100,100 );
		mpFunctionEditor->envelope->grid( 0.1f, 0.1f );
		mpFunctionEditor->control->hvalue( 0.0, 1.0, 0.0, 1.0 );
		mpFunctionEditor->control->vvalue( 0.0, 1.0, 0.0, 1.0 );
		mpFunctionEditor->envelope->margin( 20, 10, 5, 5 );
		mpFunctionEditor->end();

		// MRJ: Possible bug with Fl_Envelope_Scroll, since it seems that whenever
		// the widget is smaller than a certain critical size it is not drawn!
		/*	mpXAxisLabelBox = new Fl_Box( X+5, Y, W-10, 30 ); 
		mpXAxisLabelBox->label( mXAxisLabel.c_str() );
		mpXAxisLabelBox->labelsize( 9 );
		mpXAxisLabelBox->align( FL_ALIGN_INSIDE | FL_ALIGN_CLIP );

		mpYAxisLabelBox = new Fl_Box( X, Y+35, 30, H-60 );
		mpYAxisLabelBox->label( mYAxisLabel.c_str() );
		mpYAxisLabelBox->labelsize( 9 );
		mpYAxisLabelBox->align( FL_ALIGN_INSIDE | FL_ALIGN_CLIP );
		*/

		mpSnapToGridBtn = new Fl_Check_Button( 0,0,30,10 );
		mpSnapToGridBtn->label( "Snap points to grid" );
		mpSnapToGridBtn->labelsize( 10 );
		//mpSnapToGridBtn->align( FL_ALIGN_RIGHT );
		mpSnapToGridBtn->callback( (Fl_Callback*)cbSnapToGrid, this );


	}

	void Fl_SMS_BPF_Editor::ApplyLayoutOnContents( int X, int Y, int W, int H)
	{
		FunctionEditor().resize( X+2, Y+2, W-4, H-30 );
		FunctionEditor().margin_adjust();
		SnapToGridButton().resize( X+2, Y+H-20, W/2, 15);
	}

	void Fl_SMS_BPF_Editor::resize( int X, int Y, int W, int H )
	{
		Fl_Group::resize( X, Y, W, H );
		ApplyLayoutOnContents( x(), y(), w(), h() );
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

	void Fl_SMS_BPF_Editor::SetXAxisLabel( const char* xlabel )
	{
		mXAxisLabel = xlabel;
		CLAM_ASSERT( mpXAxisLabelBox != NULL, "X Axis label box was NULL!" );
		mpXAxisLabelBox->label( mXAxisLabel.c_str() );
		mpXAxisLabelBox->redraw();
	}

	void Fl_SMS_BPF_Editor::SetYAxisLabel( const char* ylabel )
	{
		mYAxisLabel = ylabel;
		CLAM_ASSERT( mpYAxisLabelBox != NULL, "Y Axis label box was NULL!" );
		mpYAxisLabelBox->label( mYAxisLabel.c_str() );
		mpYAxisLabelBox->redraw();
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

		clearBPF( editedBPF );
		
		int i = 0;

		for ( int i = 0; i < mpFunctionEditor->envelope->points(); i++ )
		{
			FLPOINT& p = mpFunctionEditor->envelope->point(i);
			editedBPF.Insert( p.x, p.y );
		}

		
	}
}
