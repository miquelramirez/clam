#include "Fl_SMS_BPF_Sync_Editor.hxx"
#include "Fl_Envelope_Scroll.H"
#include <FL/Fl_Check_Button.H>

namespace CLAMVM
{
	Fl_SMS_BPF_Sync_Editor::Fl_SMS_BPF_Sync_Editor( int X, int Y, int W, int H )
		: Fl_SMS_BPF_Editor( X, Y, W, H )
	{
		CreateContents();
		ApplyLayoutOnContents( X, Y, W, H );
		FunctionEditor().deactivate();
	}

	Fl_SMS_BPF_Sync_Editor::~Fl_SMS_BPF_Sync_Editor()
	{
	} 

	void Fl_SMS_BPF_Sync_Editor::CreateContents()
	{
		mpIsSynchronized = new Fl_Check_Button( 0, 0, 10, 10 );
		mpIsSynchronized->label( "Synchronized to master envelope" );
		mpIsSynchronized->value(1);
		mpIsSynchronized->labelsize( 10 );
		mpIsSynchronized->callback( (Fl_Callback*)cb_OnSyncButtonPressed, this );
		add( mpIsSynchronized );
		
	}

	void Fl_SMS_BPF_Sync_Editor::ApplyLayoutOnContents( int X, int Y, int W, int H )
	{
		FunctionEditor().resize( X+2, Y+2, W-4, H-30 );
		SnapToGridButton().resize( X+2, Y+H-20, W/2, 15 );
		mpIsSynchronized->resize( X+(W/2)+2, Y+H-20, (W/2)-2, 15 );
	}

	void Fl_SMS_BPF_Sync_Editor::cb_OnSyncButtonPressed( Fl_Check_Button* w, Fl_SMS_BPF_Sync_Editor* obj )
	{
		if ( w->value() ) 
		{
			obj->SynchronizationChanged.Emit( true );
			obj->FunctionEditor().deactivate();
			obj->FunctionEditor().redraw();
		}
		else 
		{
			obj->SynchronizationChanged.Emit( false );
			obj->FunctionEditor().activate();
			obj->FunctionEditor().redraw();
		}

	}
}
