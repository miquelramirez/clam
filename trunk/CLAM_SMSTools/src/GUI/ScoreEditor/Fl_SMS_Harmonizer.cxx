#include "Fl_SMS_Harmonizer.hxx"
#include <FL/Fl_Box.H>
#include <FL/Fl_Light_Button.H>
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_SMS_Grid_Surface.hxx"

namespace CLAMVM
{
	Fl_SMS_Harmonizer::Fl_SMS_Harmonizer( int X, int Y, int W, int H, const char* label )
		: Fl_Group( X, Y, W, H, label ), mpDisplay( NULL )
	{
		int buttonWidth = W/4;

		mAddButton = new Fl_Light_Button( X+20, Y, buttonWidth, 15 );
		mAddButton->label( "Add" );
		mAddButton->labelsize( 10 );
		mAddButton->value(1);
		mAddButton->tooltip( "Click for add new voices" );

		mRemoveButton = new Fl_Light_Button( X+20+buttonWidth, Y, buttonWidth, 15 );
		mRemoveButton->label("Remove");
		mRemoveButton->labelsize( 10 );
		mRemoveButton->value(0);
		mRemoveButton->tooltip( "Click for removing any voice" );

		mMoveButton = new Fl_Light_Button( X+20+(2*buttonWidth), Y, buttonWidth, 15 );
		mMoveButton->label( "Move" );
		mMoveButton->labelsize( 10 );
		mMoveButton->value(0);
		mMoveButton->tooltip( "Click for moving existing voices" );
	
		mpYAxis = new Fl_Y_Axis( X+5, Y+25, 40, H-60 );
		mpYAxis->label( "P.T.F." );
		mpYAxis->labelsize( 9 );
		mpYAxis->scale( FL_AXIS_LIN );
		mpYAxis->minimum( 0.5 );
		mpYAxis->maximum( 2.0 );
		mpYAxis->label_format( "%g" );
		mpYAxis->label_step( 0.1 );
		mpYAxis->label_size( 9 );
		mpYAxis->axis_color( FL_BLACK );
		mpYAxis->axis_align( FL_AXIS_LINE|FL_AXIS_LEFT  );

		mpXAxis = new Fl_X_Axis( X+45, Y+H-35, W-55, 30 );
		mpXAxis->label( "Gain Ratio (dB)" );
		mpXAxis->labelsize( 9 );
		mpXAxis->scale( FL_AXIS_LIN );
		mpXAxis->minimum( -12 );
		mpXAxis->maximum( 12 );
		mpXAxis->label_format( "%g" );
		mpXAxis->label_step( 10 );
		mpXAxis->label_size( 9 );
		mpXAxis->axis_color( FL_BLACK );
		mpXAxis->axis_align( FL_AXIS_BOTTOM|FL_AXIS_LINE);

		mpImposter = new Fl_Box( X+45, Y+25, W-55, H-60 );
		mpImposter->label( "Wait..." );
		resizable( mpImposter );

		end();
		
		mWSC.mLeft = -12;
		mWSC.mRight = 12;
		mWSC.mTop = 2.0;
		mWSC.mBottom = 0.5;

	}

	Fl_SMS_Harmonizer::~Fl_SMS_Harmonizer()
	{
	}

	int Fl_SMS_Harmonizer::handle( int event )
	{
		if ( event == FL_SHOW )
		{
			mpDisplay = new Fl_SMS_Grid_Surface( x()+45, y()+25, w()-55, h()-60 );
			mpDisplay->EnableDoubleBuffering();
			mpDisplay->SetWorldSpace( mWSC.mRight, mWSC.mLeft, mWSC.mTop, mWSC.mBottom );
			mpDisplay->end();
			mpDisplay->SetGridWidth( 3, 0.1 );
			add(mpDisplay);
			resizable( mpDisplay );
			remove(*mpImposter);
			mAddButton->resize( x()+ (w()/4), y(), w()/4, 15 );
			mRemoveButton->resize( x() + (2*(w()/4)), y(), w()/4, 15 );
			mMoveButton->resize( x() + (3*(w()/4)), y(), w()/4, 15 );
		}
		else if ( event == FL_HIDE )
		{
			if ( mpDisplay )
			{
				remove( mpDisplay );
				delete mpDisplay;
				mpDisplay = NULL;
			}
			add( mpImposter );
			resizable( mpImposter );
		}

		return Fl_Group::handle( event );
	}

	void Fl_SMS_Harmonizer::Show()
	{
	}

	void Fl_SMS_Harmonizer::Hide()
	{
	}

}
