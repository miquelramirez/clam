#ifndef __SMS_PEAKCONTINUATIONSETTINGS__
#define __SMS_PEAKCONTINUATIONSETTINGS__

#include <FL/Fl_Group.H>

class Fl_Input;
class Fl_Output;
class Fl_Box;
class Fl_Check_Button;
class Fl_Counter;

namespace SMS
{

namespace ToolsUI
{
	class PeakContinuationSettings
		: public Fl_Group
	{
	public:
		PeakContinuationSettings();
		virtual ~PeakContinuationSettings();

	protected:

		void resize( int X, int Y, int W, int H );
		void BuildContents();
		void ArrangeContentsGeometry( int X, int Y, int W, int H );
	protected:

		Fl_Counter*         mMaxFreqDevFactor;
		Fl_Box*             mMaxFreqDecorator;
		Fl_Check_Button*    mAssumeHarmonicity;
		Fl_Box*             mAssHarmDecorator;
		Fl_Check_Button*    mActivateTrackCleaning;
		Fl_Box*             mActTrackCleanDecorator;
		Fl_Group*           mTrackPruningGroup;
		Fl_Output*          mExpectedFrames;
		Fl_Output*          mFrameLen;
		Fl_Box*             mFrameLenDecorator;
		Fl_Input*           mToleranceToDiscont;
		Fl_Box*             mTTDDecorator;
		Fl_Input*           mMinTrackLen;
		Fl_Box*             mMinTrackLenDecorator;
	};
}

}

#endif // SMS_PeakContinuationSettings.hxx
