#include "MessageBox.hxx"
#include <FL/fl_message.H>
#include <FL/Fl.H>

namespace CLAMVM
{
	MessageBox::MessageBox()
		: mMessageText( "No message" )
	{
	}

	MessageBox::~MessageBox()
	{
	}

	void MessageBox::SetText( const std::string& msgTxt )
	{
		mMessageText = msgTxt;
	}

	void MessageBox::Show()
	{
		fl_message( mMessageText.c_str() );
		Fl::flush();
	}
	
}
