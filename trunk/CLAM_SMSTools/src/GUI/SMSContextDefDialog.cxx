#include "SMSContextDefDialog.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>

namespace SMS
{

namespace ToolsUI
{
	SMSContextDefDialog::SMSContextDefDialog( int X, int Y, int W, int H )
		: Fl_Window( X, Y, W, H )
	{
		label( "SMS Process Context Definition" );

		BuildContents();
		end();
		ArrangeContentsGeometry(0,0,W,H);
	}

	SMSContextDefDialog::~SMSContextDefDialog()
	{
	}

	void SMSContextDefDialog::Tabulate( Fl_Widget* pWidget )
	{
		mParamsFamiliesTabs->add( pWidget );
		mParamsFamiliesTabs->resizable(  pWidget );
	}

	void SMSContextDefDialog::BuildContents()
	{
		mActionButtonsContainer = new Fl_Group( 0, 0, 30, 30 );
		{
			mProceedAction = new Fl_Button( 0, 0, 30, 30 );
			mProceedAction->label( "I am finished with defining this context" );
			mProceedAction->box( FL_THIN_UP_BOX );
			mProceedAction->labelsize( 10 );
		}
		{
			mCancelAction = new Fl_Button( 0, 0, 30, 30 );
			mCancelAction->label( "I no longer want to define a new context" );
			mCancelAction->box( FL_THIN_UP_BOX );
			mCancelAction->labelsize( 10 );
		}
		{
			mHelpRequestedAction = new Fl_Button(540, 390, 125, 25);
			mHelpRequestedAction->label( "I need help" );
			mHelpRequestedAction->box( FL_THIN_UP_BOX );
			mHelpRequestedAction->labelsize( 10 );
		}
		mActionButtonsContainer->color( 50 );
		mActionButtonsContainer->end();

		mParamsFamiliesTabs = new Fl_Tabs( 0, 0, 665, 380);
		mParamsFamiliesTabs->end();
	}

	void SMSContextDefDialog::resize( int X, int Y, int W, int H )
	{
		Fl_Group::resize( X, Y, W, H );
		ArrangeContentsGeometry( 0, 0, W, H );
	}

	void SMSContextDefDialog::ArrangeContentsGeometry( int X, int Y, int W, int H )
	{
		// TODO: width for buttons should be enforced?
		int buttonOffset=X;
		mActionButtonsContainer->resize( X, H - 50, W, 40 );
		int buttonWidth = int(float(W)/2.56f);
		mProceedAction->resize( buttonOffset, H-35, buttonWidth, 25 );
		buttonOffset+=buttonWidth + 4;
		buttonWidth = int( float(W)/2.45f);
		mCancelAction->resize( buttonOffset, H-35, buttonWidth, 25 );
		buttonOffset+=buttonWidth + 4;
		buttonWidth = W/5.32;
		mHelpRequestedAction->resize( buttonOffset, H-35, buttonWidth, 25 );
		mParamsFamiliesTabs->resize( X, Y, W, H-40 );
	}
}

}
