#ifndef __SMS_SPPPANALYSISSETTINGS__
#define __SMS_SPPPANALYSISSETTINGS__

#include <FL/Fl_Group.H>

class Fl_Input;
class Fl_Box;
class Fl_Counter;

namespace SMS
{

namespace ToolsUI
{
	class SPpPAnalysisSettings
		: public Fl_Group
	{
	public:
		SPpPAnalysisSettings();
		virtual ~SPpPAnalysisSettings();
		
	protected:
		
		void resize( int X, int Y, int W, int H );
		void BuildContents();
		void ArrangeContentsGeometry( int X, int Y, int W, int H );
	protected:

		Fl_Input*      mPeakMagnitudeThreshold;
		Fl_Box*        mPeakMagThresDecorator;
		Fl_Input*      mPeakFreqCutoff;
		Fl_Box*        mPeakFreqCutoffDecorator;
		Fl_Group*      mFreqRangeGroup;
		Fl_Box*        mLowerBoundFreqDecorator;
		Fl_Box*        mUpperBoundDecorator;
		Fl_Counter*    mLowerBoundFreq;
		Fl_Counter*    mUpperBoundFreq;
	};
}

}

#endif // SMS_SPpPAnalysisSettings.hxx
