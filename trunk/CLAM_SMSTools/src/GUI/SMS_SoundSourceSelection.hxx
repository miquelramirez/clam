#ifndef __SMS_SOUNDSOURCESELECTION__
#define __SMS_SOUNDSOURCESELECTION__

#include <FL/Fl_Group.H>

class Fl_File_Input;
class Fl_Button;
class Fl_Output;

namespace SMS
{

namespace ToolsUI
{
	class SoundSourceSelection
		: public Fl_Group
	{
	public:

		SoundSourceSelection();
		virtual ~SoundSourceSelection();

	protected:

		void resize( int X, int Y, int W, int H );

		void BuildContents();
		void ArrangeContentsGeometry( int X, int Y, int W, int H );

	protected:
		Fl_File_Input*   mFilenameCollector;
		Fl_Button*       mBrowseFileSystem;
		Fl_Group*        mSignalThumbnail;
		Fl_Group*        mSignalParametersContainer;
		Fl_Group*        mPlaybackCtlsContainer;
		Fl_Output*       mSampleRate;
		Fl_Output*       mSampleResolution;
		Fl_Output*       mDuration;
		Fl_Button*       mPlaySound;
		Fl_Button*       mStopSound;
		
	};
}

}

#endif // SMS_SoundSourceSelection.hxx
