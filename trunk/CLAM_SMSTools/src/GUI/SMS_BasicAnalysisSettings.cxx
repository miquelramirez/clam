#include "SMS_BasicAnalysisSettings.hxx"
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Value_Slider.H>

namespace SMS
{

namespace ToolsUI
{
	BasicAnalysisSettings::BasicAnalysisSettings()
		: Fl_Group( 0, 0, 497, 364 )
	{
		label( "Basic Analysis Settings" );
		labelsize( 10 );

		BuildContents();
		
		end();

		ArrangeContentsGeometry( 0, 0, 497, 364 );
	}

	BasicAnalysisSettings::~BasicAnalysisSettings()
	{
	}

	void BasicAnalysisSettings::resize( int X, int Y, int W, int H )
	{
		Fl_Group::resize( X, Y, W, H );
		ArrangeContentsGeometry( 0, 0, W, H );
	}

	void BasicAnalysisSettings::BuildContents()
	{
		int xOffset = 50;

		{
			mAnalysisWindowWidth = new Fl_Input(xOffset + 10, 25, 110, 25 );
			mAnalysisWindowWidth->label( "Analysis Window Width: " );
			mAnalysisWindowWidth->labelsize(12);
			mAnalysisWindowWidth->textsize(12);
			mAnalysisWindowWidth->align(FL_ALIGN_TOP_LEFT);
		}
		{
			mFrameLengthUnit = new Fl_Box(xOffset + 120, 25, 25, 25, "ms");
			mFrameLengthUnit->labelsize(12);
			mFrameLengthUnit->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}
		{
			mOverlapFactor = new Fl_Value_Slider(xOffset + 180, 25, 170, 25, "Window Overlap Factor:");
			mOverlapFactor->type(1);
			mOverlapFactor->labelsize(12);
			mOverlapFactor->textsize(12);
			mOverlapFactor->align(FL_ALIGN_TOP_LEFT);
		}
		{
			mOverlapUnit = new Fl_Box(xOffset + 150, 25, 25, 25, "%");
			mOverlapUnit->labelsize(12);
			mOverlapUnit->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
		}
		{
			mZeroPaddingFactor = new Fl_Counter(xOffset + 365, 25, 120, 25, "Zero Padding Factor:");
			mZeroPaddingFactor->labelsize(12);
			mZeroPaddingFactor->minimum(0);
			mZeroPaddingFactor->maximum(16);
			mZeroPaddingFactor->step(1);
			mZeroPaddingFactor->textsize(12);
			mZeroPaddingFactor->align(FL_ALIGN_TOP_LEFT);
		}
		{
			mWindowFunction = new Fl_Choice(xOffset + 11, 120, 314, 25, "Window function to be used:");
			mWindowFunction->down_box(FL_BORDER_BOX);
			mWindowFunction->labelsize(12);
			mWindowFunction->textsize(12);
			mWindowFunction->align(FL_ALIGN_TOP_LEFT);
		}
		{
			mTDFunctionContainer = new Fl_Group(xOffset + 2, 165, 240, 195, "Time-domain window function shape");
			mTDFunctionContainer->box(FL_ENGRAVED_FRAME);
			mTDFunctionContainer->labelfont(1);
			mTDFunctionContainer->labelsize(12);
			mTDFunctionContainer->end();
		}
		{
			mFDFunctionContainer = new Fl_Group(xOffset + 253, 165, 240, 195, "Frequency-domain window function shape");
			mFDFunctionContainer->box(FL_ENGRAVED_FRAME);
			mFDFunctionContainer->labelfont(1);
			mFDFunctionContainer->labelsize(12);
			mFDFunctionContainer->end();
		}
		{
			mBrowseWithWindow = new Fl_Button(xOffset + 330, 120, 162, 25, "Browse audio with window...");
			mBrowseWithWindow->box(FL_THIN_UP_BOX);
			mBrowseWithWindow->labelsize(12);
		}
		{
			mEffectiveSinSize = new Fl_Output(xOffset + 9, 75, 97, 30, "Samples used for Sinusoidal Analysis:");
			mEffectiveSinSize->box(FL_NO_BOX);
			mEffectiveSinSize->labelsize(12);
			mEffectiveSinSize->textsize(12);
			mEffectiveSinSize->align(FL_ALIGN_TOP_LEFT);
		}
		{
			mEffectiveSinSizeDecorator = new Fl_Box(xOffset + 108, 75, 69, 30, "samples");
			mEffectiveSinSizeDecorator->labelsize(12);
			mEffectiveSinSizeDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}
		{
			mEffectiveResSize = new Fl_Output(xOffset + 241, 75, 81, 30, "Samples used for Residual Analysis:");
			mEffectiveResSize->box(FL_NO_BOX);
			mEffectiveResSize->labelsize(12);
			mEffectiveResSize->textsize(12);
			mEffectiveResSize->align(FL_ALIGN_TOP_LEFT);
		}
		{
			mEffectiveResSizeDecorator = new Fl_Box(xOffset + 330, 75, 69, 30, "samples");
			mEffectiveResSizeDecorator->labelsize(12);
			mEffectiveResSizeDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}
	}

	void BasicAnalysisSettings::ArrangeContentsGeometry( int X, int Y, int W, int H )
	{
		
	}

}

}
