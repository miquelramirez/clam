#ifndef __SMSCONTEXTDEFDIALOG__
#define __SMSCONTEXTDEFDIALOG__

#include <FL/Fl_Window.H>

// forward declarations
class Fl_Group;
class Fl_Button;
class Fl_Tabs;

namespace SMS
{

namespace ToolsUI
{
	class SMSContextDefDialog // SMS Context Definition Dialog class
		: public Fl_Window
	{
	public:
		SMSContextDefDialog( int X, int Y, int W, int H );
		virtual ~SMSContextDefDialog();

		void Tabulate( Fl_Widget* pWidget );

	protected:
		void resize( int X, int Y, int W, int H );
			       
		void BuildContents();
		void ArrangeContentsGeometry( int X, int Y, int W, int H );

	protected:
		Fl_Group*   mActionButtonsContainer;
		Fl_Button*  mProceedAction;
		Fl_Button*  mCancelAction;
		Fl_Button*  mHelpRequestedAction;
		Fl_Tabs*    mParamsFamiliesTabs;
	};
}

}


#endif // SMSContextDefDialog.hxx
