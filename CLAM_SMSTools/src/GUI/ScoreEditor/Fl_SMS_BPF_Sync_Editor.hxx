#ifndef __FL_SMS_BPF_SYNC_EDITOR__
#define __FL_SMS_BPF_SYNC_EDITOR__

#include "Fl_SMS_BPF_Editor.hxx"
#include "Signalv1.hxx"

class Fl_Check_Button;

namespace CLAMVM
{
	class Fl_SMS_BPF_Sync_Editor : public Fl_SMS_BPF_Editor
	{
	public:
		Fl_SMS_BPF_Sync_Editor( int X, int Y, int W, int H );
		virtual ~Fl_SMS_BPF_Sync_Editor();

		SigSlot::Signalv1<bool>   SynchronizationChanged;
		
	protected:

		void CreateContents();
		void ApplyLayoutOnContents( int X, int Y, int W, int H );

		static void cb_OnSyncButtonPressed( Fl_Check_Button*, Fl_SMS_BPF_Sync_Editor* );

	protected:
		Fl_Check_Button*    mpIsSynchronized;

	};
}

#endif // Fl_SMS_BPF_Sync_Editor.hxx
