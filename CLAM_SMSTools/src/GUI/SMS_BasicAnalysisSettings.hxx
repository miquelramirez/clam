#ifndef __SMS_BASICANALYSISSETTINGS__
#define __SMS_BASICANALYSISSETTINGS__

#include <FL/Fl_Group.H>

// forward declarations
class Fl_Input;
class Fl_Box;
class Fl_Counter;
class Fl_Choice;
class Fl_Output;
class Fl_Value_Slider;
class Fl_Button;

namespace SMS
{

namespace ToolsUI
{
	class BasicAnalysisSettings 
		: public Fl_Group
	{
	public:
		BasicAnalysisSettings( );
		virtual ~BasicAnalysisSettings();
		
		void resize( int X, int Y, int W, int H );
	protected:

		void BuildContents();
		void ArrangeContentsGeometry( int X, int Y, int W, int H );

	protected:
		Fl_Input*          mAnalysisWindowWidth;
		Fl_Box*            mFrameLengthUnit;
		Fl_Value_Slider*   mOverlapFactor;
		Fl_Box*            mOverlapUnit;
		Fl_Counter*        mZeroPaddingFactor;
		Fl_Choice*         mWindowFunction;
		Fl_Group*          mTDFunctionContainer;
		Fl_Group*          mFDFunctionContainer;
		Fl_Button*         mBrowseWithWindow;
		Fl_Output*         mEffectiveSinSize;
		Fl_Output*         mEffectiveResSize;
		Fl_Box*            mEffectiveSinSizeDecorator;
		Fl_Box*            mEffectiveResSizeDecorator;
	};

}

}

#endif // SMS_BasicAnalysisSettings.hxx
