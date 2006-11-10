#include "SMS_SPpPAnalysisSettings.hxx"

#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Counter.H>

namespace SMS
{

namespace ToolsUI
{
	SPpPAnalysisSettings::SPpPAnalysisSettings()
		: Fl_Group( 0, 0, 497, 365 )
	{
		label( "Peaks And Pitch Detection Settings" );
		labelsize( 10 );

		BuildContents();

		end();

		ArrangeContentsGeometry( 0, 0, 497, 365 );
	}

	SPpPAnalysisSettings::~SPpPAnalysisSettings()
	{
	}

	void SPpPAnalysisSettings::resize( int X, int Y, int W, int H )
	{
		Fl_Group::resize( X, Y, W, H );
		ArrangeContentsGeometry( 0, 0, W, H );
	}

	void SPpPAnalysisSettings::BuildContents()
	{
		{
			mPeakMagnitudeThreshold = new Fl_Input(10, 40, 110, 25, " Peak Magnitude Threshold:");
			mPeakMagnitudeThreshold->labelsize(12);
			mPeakMagnitudeThreshold->textsize(12);
			mPeakMagnitudeThreshold->align(FL_ALIGN_TOP_LEFT);
		}
		{
			mPeakMagThresDecorator = new Fl_Box(120, 40, 46, 25, "dB");
			mPeakMagThresDecorator->labelsize(12);
			mPeakMagThresDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}
		{
			mPeakFreqCutoff = new Fl_Input(12, 90, 109, 25, "Peak Detection Cutoff Frequency:");
			mPeakFreqCutoff->labelsize(12);
			mPeakFreqCutoff->textsize(12);
			mPeakFreqCutoff->align(FL_ALIGN_TOP_LEFT);
		}
		{
			mPeakFreqCutoffDecorator = new Fl_Box(123, 90, 46, 25, "Hz");
			mPeakFreqCutoffDecorator->labelsize(12);
			mPeakFreqCutoffDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}
		{
			mFreqRangeGroup = new Fl_Group(13, 140, 163, 185, "Pitch Search Range");
			mFreqRangeGroup->box(FL_ENGRAVED_FRAME);
			mFreqRangeGroup->labelfont(1);
			mFreqRangeGroup->labelsize(12);
			
			{
				mLowerBoundFreqDecorator = new Fl_Box(146, 180, 24, 25, "Hz");
				mLowerBoundFreqDecorator->labelsize(12);
				mLowerBoundFreqDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
			}
			{
				mUpperBoundDecorator = new Fl_Box(148, 255, 26, 25, "Hz");
				mUpperBoundDecorator->labelsize(12);
				mUpperBoundDecorator->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
			}
			{
				mLowerBoundFreq = new Fl_Counter(20, 180, 125, 25, "Lower bound frequency:");
				mLowerBoundFreq->labelsize(12);
				mLowerBoundFreq->minimum(0);
				mLowerBoundFreq->maximum(22050);
				mLowerBoundFreq->step(10);
				mLowerBoundFreq->value(50);
				mLowerBoundFreq->textsize(12);
				mLowerBoundFreq->align(FL_ALIGN_TOP_LEFT);
			}
			{
				mUpperBoundFreq = new Fl_Counter(19, 255, 128, 25, "Upper bound frequency:");
				mUpperBoundFreq->labelsize(12);
				mUpperBoundFreq->minimum(0);
				mUpperBoundFreq->maximum(22050);
				mUpperBoundFreq->step(10);
				mUpperBoundFreq->value(6000);
				mUpperBoundFreq->textsize(12);
				mUpperBoundFreq->align(FL_ALIGN_TOP_LEFT);
			}

			mFreqRangeGroup->end();
		}
	}

	void SPpPAnalysisSettings::ArrangeContentsGeometry( int X, int Y, int W, int H )
	{
	}
}

}
