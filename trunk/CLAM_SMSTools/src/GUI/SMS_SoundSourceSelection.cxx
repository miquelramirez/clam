#include "SMS_SoundSourceSelection.hxx"
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>

namespace SMS
{

namespace ToolsUI
{
	SoundSourceSelection::SoundSourceSelection()
		: Fl_Group( 0, 0, 498, 364 )
	{
		label( "Input signal selection" );
		labelsize(10);
		BuildContents();
		ArrangeContentsGeometry( 0, 0, 498, 364 );	

	}

	SoundSourceSelection::~SoundSourceSelection()
	{
	}

	void SoundSourceSelection::BuildContents()
	{
		{
			mFilenameCollector = new Fl_File_Input( 2, 25, 360, 35 );
			mFilenameCollector->label( "Audio file containing input signal:" );
			mFilenameCollector->align( FL_ALIGN_LEFT|FL_ALIGN_TOP);
			mFilenameCollector->box(FL_THIN_UP_BOX);
			mFilenameCollector->labelsize(12);
		}
		{
			mBrowseFileSystem = new Fl_Button(368, 35, 130, 25 );
			mBrowseFileSystem->label( "Browse filesystem..." );
			mBrowseFileSystem->box(FL_THIN_UP_BOX);
			mBrowseFileSystem->labelsize(12);
		}
		{
			mSignalThumbnail = new Fl_Group( 1, 75, 364, 285 );
			mSignalThumbnail->label("No signal was selected" );
			mSignalThumbnail->box(FL_ENGRAVED_FRAME);
			mSignalThumbnail->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
			mSignalThumbnail->end();
			resizable( mSignalThumbnail );
		}

		{
			mSignalParametersContainer = new Fl_Group(375, 190, 120, 25 );
			mSignalParametersContainer->label( "Audio attributes" );
			mSignalParametersContainer->box(FL_ENGRAVED_BOX);
			mSignalParametersContainer->labelfont(1);
			mSignalParametersContainer->labelsize(12);
			mSignalParametersContainer->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
			{
				mSampleRate = new Fl_Output(375, 140, 120, 25 );
				mSampleRate->label( "Sample Rate:" );
				mSampleRate->box(FL_NO_BOX);
				mSampleRate->labelsize(10);
				mSampleRate->textsize(10);
				mSampleRate->align(FL_ALIGN_TOP_LEFT);
				mSignalParametersContainer->resizable( mSampleRate );
			}
			{
				mSampleResolution = new Fl_Output(375, 190, 120, 25 );
				mSampleResolution->label( "Sample Resolution:" );
				mSampleResolution->box(FL_NO_BOX);
				mSampleResolution->labelsize(10);
				mSampleResolution->textsize(10);
				mSampleResolution->align(FL_ALIGN_TOP_LEFT);
				mSignalParametersContainer->resizable( mSampleResolution );
			}
			{
				mDuration = new Fl_Output(375, 245, 120, 25 );
				mDuration->label( "Duration:" );
				mDuration->box(FL_NO_BOX);
				mDuration->labelsize(10);
				mDuration->textsize(10);
				mDuration->align(FL_ALIGN_TOP_LEFT);
				mSignalParametersContainer->resizable( mDuration );
			}
			mSignalParametersContainer->end();
		}
		{
			mPlaybackCtlsContainer = new Fl_Group(370, 285, 128, 70 );
			mPlaybackCtlsContainer->label( "Playback controls" );
			mPlaybackCtlsContainer->box(FL_ENGRAVED_FRAME);
			mPlaybackCtlsContainer->labelfont(1);
			mPlaybackCtlsContainer->labelsize(12);
			mPlaybackCtlsContainer->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
			{
				{
					mPlaySound = new Fl_Button(380, 315, 40, 30 );
					mPlaySound->label( "Play" );
					mPlaySound->box(FL_OSHADOW_BOX);
					mPlaySound->down_box(FL_OFLAT_BOX);
					mPlaySound->labelsize(10);
				}
				{
					mStopSound = new Fl_Button(440, 315, 40, 30 );
					mStopSound->label( "Stop" );
					mStopSound->box(FL_OSHADOW_BOX);
					mStopSound->down_box(FL_OFLAT_BOX);
					mStopSound->labelsize(10);
				}
			}
			mPlaybackCtlsContainer->end();
		}

		end();		
	}

	void SoundSourceSelection::resize( int X, int Y, int W, int H )
	{
		Fl_Group::resize( X, Y, W, H );
		ArrangeContentsGeometry( 0, 0, W, H );
	}

	void SoundSourceSelection::ArrangeContentsGeometry( int X, int Y, int W, int H )
	{	
		mFilenameCollector->resize( X+2, Y+25, W-50, 35 );
		mBrowseFileSystem->resize( W-48, Y+35, 120, 25 );
		mSignalThumbnail->resize( X+1, Y+75, W-75, H-Y-80 );		
		
		mSignalParametersContainer->resize( W-73, Y+75, 145, 160 );
		{
			int subX = W-73+2, subY = Y + 75, subW = 145, subH = 160;

			mSampleRate->resize( subX+2, subY+50, mSampleRate->parent()->w()-4, 25 );
			
			mSampleResolution->resize( subX+2, subY+80, mSampleResolution->parent()->w()-4, 25 );

			mDuration->resize( subX+2, subY + 110, mDuration->parent()->w()-4, 25 );
			
		}
		mPlaybackCtlsContainer->resize( W-73, Y+237, 145, H-242 );
		{
			int totalAvailableWidth = mPlaybackCtlsContainer->w()-10;
			
			mPlaySound->resize( mPlaybackCtlsContainer->x()+2,
					    mPlaybackCtlsContainer->y()+50,
					    totalAvailableWidth/2 - 2,
					    40 );

			mStopSound->resize( mPlaybackCtlsContainer->x()+4+(totalAvailableWidth/2),
					    mPlaybackCtlsContainer->y()+50,
					    totalAvailableWidth/2 - 2,
					    40 );
			
		}
	}
}

}
