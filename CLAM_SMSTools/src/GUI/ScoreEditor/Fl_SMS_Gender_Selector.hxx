#ifndef __FL_SMS_GENDER_SELECTOR__
#define __FL_SMS_GENDER_SELECTOR__

#include <FL/Fl_Group.H>
#include "Signalv1.hxx"
#include "Slotv1.hxx"

class Fl_Round_Button;

namespace CLAMVM
{

	class Fl_SMS_Gender_Selector : public Fl_Group
	{
	public:
		Fl_SMS_Gender_Selector( int X, int Y, int W, int H );
		virtual ~Fl_SMS_Gender_Selector();

		SigSlot::Signalv1<int>   GenderSelected;
		SigSlot::Slotv1<int>     SetSelectedGender;

	protected:

		void OnSelectedGenderSet( int gender );

		static void cbMaleFemale( Fl_Round_Button* btn, void* data );

	protected:

		Fl_Round_Button*   mpMaleButton;
		Fl_Round_Button*   mpFemaleButton;


		static const char* mpMaleButtonLabel;
		static const char* mpFemaleButtonLabel;
	};

}

#endif // Fl_SMS_Gender_Selector.hxx
