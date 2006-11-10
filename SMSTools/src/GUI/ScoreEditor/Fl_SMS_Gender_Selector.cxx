#include "Fl_SMS_Gender_Selector.hxx"
#include <FL/Fl_Round_Button.H>

namespace CLAMVM
{
	const char* Fl_SMS_Gender_Selector::mpMaleButtonLabel= "Change a male voice into a female one";
	const char* Fl_SMS_Gender_Selector::mpFemaleButtonLabel = "Change a female voice into a male one";

	Fl_SMS_Gender_Selector::Fl_SMS_Gender_Selector( int X, int Y, int W, int H )
		: Fl_Group( X, Y, W, H )
	{
		mpMaleButton = new Fl_Round_Button( X+(W/4), Y+(H/4), W/2, 20 );
		mpMaleButton->label( mpMaleButtonLabel );
		mpMaleButton->labelsize( 12 );
		mpMaleButton->when( FL_WHEN_RELEASE );
		mpMaleButton->callback( (Fl_Callback*)cbMaleFemale, this );
		mpMaleButton->value(1);

		mpFemaleButton = new Fl_Round_Button( X+(W/4), Y+(H/4)+25, W/2, 20 );
		mpFemaleButton->label( mpFemaleButtonLabel );
		mpFemaleButton->labelsize( 12 );
		mpFemaleButton->when( FL_WHEN_RELEASE );
		mpFemaleButton->callback( (Fl_Callback*)cbMaleFemale, this );
		mpFemaleButton->value(0);

		end();

		SetSelectedGender.Wrap( this, &Fl_SMS_Gender_Selector::OnSelectedGenderSet );
	}

	Fl_SMS_Gender_Selector::~Fl_SMS_Gender_Selector()
	{
	}

	void Fl_SMS_Gender_Selector::OnSelectedGenderSet( int gender )
	{
		if ( gender==0 ) // male selected
		{
			mpMaleButton->value(1);
			mpFemaleButton->value(0);
			redraw();
		}
		else
		{
			mpMaleButton->value(0);
			mpFemaleButton->value(1);
			redraw();
		}
	}

	void Fl_SMS_Gender_Selector::cbMaleFemale( Fl_Round_Button* btn, void* data )
	{
		Fl_SMS_Gender_Selector* pThis = ( Fl_SMS_Gender_Selector* )data;
		
		if ( btn == pThis->mpMaleButton ) // male button was clicked
		{
			pThis->mpFemaleButton->value(0);
			pThis->GenderSelected.Emit( 0 );
		}
		else if ( btn == pThis->mpFemaleButton )
		{
			pThis->mpMaleButton->value(0);
			pThis->GenderSelected.Emit( 1 );
		}
	}
}
