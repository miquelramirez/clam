#ifndef __FL_SMS_SINTRACKS_BROWSER__
#define __FL_SMS_SINTRACKS_BROWSER__

#include <string>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "DataTypes.hxx"
#include "SinTracksPresentation.hxx"
#include "SinTracksRenderingManager.hxx"
#include "SineTracksDef.hxx"
#include "Signalv1.hxx"
#include "Slotv0.hxx"
#include "Slotv1.hxx"
#include "TooltipTracker2D.hxx"

namespace CLAMVM
{
	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_SMS_Gl_Single_Browsable_Display;

	class Fl_SMS_SinTracks_Browser 
		: public SinTracksPresentation, public Fl_Group
	{
	private:
		Fl_X_Axis*                          mXAxis;
		Fl_Y_Axis*                          mYAxis;
		Fl_ZoomSlider*                      mXSlider;
		Fl_ZoomSlider*                      mYSlider;
		Fl_SMS_Gl_Single_Browsable_Display* mDisplay;
		SinTracksRM                         mDrawMgr;
		CLAM::TTime                         mBeginTime;
		CLAM::TTime                         mEndTime;
		CLAM::TSize                         mFrames;
		Fl_Box*                             mImposterBox;
		DataBoundBox                        mWorldSpaceCoords;
		std::string                         mTooltipFmtStr;
		double                              mMaxFreq;
		double                              mMinFreq;

	protected:

		virtual void OnNewTrackList( SineTrackList& list, TSize framelen ); 
		
		virtual void OnNewRange( TData );
		
		virtual void OnNewDuration( TTime begin, TTime end );		

		virtual void OnDisplaySelectedXValue( double value );
		virtual void OnSetSelectedXValue( double value );

		SigSlot::Signalv1< double > ChangeSelectedXValue;
		SigSlot::Slotv1< double >   HandleDisplaySelection;

		void OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen ); 

		TooltipTracker2D   mTooltipTracker;

	public:
		
		Fl_SMS_SinTracks_Browser( int X, int Y, int W, int H, const char* label = 0 );
		~Fl_SMS_SinTracks_Browser();
		virtual void Show();
		virtual void Hide();

		int handle( int event );

		SigSlot::Slotv1<double>        SetSelectedXValue;
		SigSlot::Signalv1<double>      SelectedXValue;

		void SetTooltipFormat( const char* fmtStr );
		void SetYRange( double ymin, double ymax );
		
	};

	// inlines
	inline void Fl_SMS_SinTracks_Browser::SetTooltipFormat( const char* fmtStr )
	{
		mTooltipFmtStr = fmtStr;
	}
}

#endif // Fl_SMS_SinTracks_Browser.hxx
