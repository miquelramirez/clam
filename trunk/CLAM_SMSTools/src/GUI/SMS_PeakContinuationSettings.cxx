#include "SMS_PeakContinuationSettings.hxx"

#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Counter.H>

namespace SMS
{

namespace ToolsUI
{
	PeakContinuationSettings::PeakContinuationSettings()
		: Fl_Group( 0, 0, 498, 362 )
	{
		label( "Peak Continuation Settings" );
		labelsize( 10 );

		BuildContents();

		end();

		ArrangeContentsGeometry( 0, 0, 498, 362 );
	}

	PeakContinuationSettings::~PeakContinuationSettings()
	{
	}

	void PeakContinuationSettings::resize( int X, int Y, int W, int H )
	{
		Fl_Group::resize( X, Y, W, H );
		ArrangeContentsGeometry( 0, 0, W, H );
	}
	
	void PeakContinuationSettings::BuildContents()
	{
		{
			mMaxFreqDevFactor = new Fl_Counter(18, 35, 86, 25, "Maximum allowed frequency deviation:");
			mMaxFreqDevFactor->labelsize(12);
			mMaxFreqDevFactor->minimum(0);
			mMaxFreqDevFactor->maximum(300);
			mMaxFreqDevFactor->step(10);
			mMaxFreqDevFactor->value(20);
			mMaxFreqDevFactor->textsize(10);
			mMaxFreqDevFactor->align(FL_ALIGN_TOP_LEFT);			
		}
		{
			mMaxFreqDecorator = new Fl_Box(106, 35, 30, 25, "%");
			mMaxFreqDecorator->labelsize(12);
			mMaxFreqDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);			
		}
		{
			mAssumeHarmonicity = new Fl_Check_Button(18, 75, 26, 25);
			mAssumeHarmonicity->down_box(FL_DOWN_BOX);
			mAssumeHarmonicity->labelsize(13);			
		}
		{
			mAssHarmDecorator = new Fl_Box(44, 75, 188, 25, "Assume input signal harmonicity");
			mAssHarmDecorator->labelsize(12);
			mAssHarmDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);			
		}
		{
			mActivateTrackCleaning = new Fl_Check_Button(18, 115, 25, 25);
			mActivateTrackCleaning->down_box(FL_DOWN_BOX);
			mActivateTrackCleaning->labelsize(13);
		}
		{
			mActTrackCleanDecorator = new Fl_Box(43, 115, 218, 25, "Activate sinusoidal tracks pruning");
			mActTrackCleanDecorator->labelsize(12);
			mActTrackCleanDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);			
		}
		{
			mTrackPruningGroup = new Fl_Group(6, 140, 488, 120);
			mTrackPruningGroup->box(FL_ENGRAVED_FRAME);
			{
				mExpectedFrames = new Fl_Output(21, 165, 124, 25, "Number of frames to be processed:");
				mExpectedFrames->box(FL_NO_BOX);
				mExpectedFrames->labelsize(12);
				mExpectedFrames->textsize(12);
				mExpectedFrames->align(FL_ALIGN_TOP_LEFT);
			}
			{
				mFrameLen = new Fl_Output(256, 165, 74, 25, "Frame length:");
				mFrameLen->box(FL_NO_BOX);
				mFrameLen->labelsize(12);
				mFrameLen->textsize(12);
				mFrameLen->align(FL_ALIGN_TOP_LEFT);
			}
			{
				mFrameLenDecorator = new Fl_Box(330, 165, 87, 25, "ms");
				mFrameLenDecorator->labelsize(12);
				mFrameLenDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
			}
			{
				mToleranceToDiscont = new Fl_Input(23, 215, 73, 25, "Tolerance to track discontinuation:");
				mToleranceToDiscont->labelsize(12);
				mToleranceToDiscont->textsize(12);
				mToleranceToDiscont->align(FL_ALIGN_TOP_LEFT);
			}
			{
				mTTDDecorator = new Fl_Box(96, 215, 73, 25, "frames");
				mTTDDecorator->labelsize(12);
				mTTDDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);				
			}
			{
				mMinTrackLen = new Fl_Input(252, 215, 72, 25, "Minimum track length:");
				mMinTrackLen->labelsize(12);
				mMinTrackLen->textsize(12);
				mMinTrackLen->align(FL_ALIGN_TOP_LEFT);
			}
			mTrackPruningGroup->end();
		}
		{
			mMinTrackLenDecorator = new Fl_Box(330, 215, 73, 25, "frames");
			mMinTrackLenDecorator->labelsize(12);
			mMinTrackLenDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}

	}

	void PeakContinuationSettings::ArrangeContentsGeometry( int X, int Y, int W, int H )
	{
	}
}

}
