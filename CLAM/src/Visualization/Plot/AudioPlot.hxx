#ifndef __AUDIOPLOT__
#define __AUDIOPLOT__

#include "PlotBase.hxx"

namespace CLAM
{
	class Audio;
}

namespace CLAMVM
{
	class AudioAdapter;
	class Fl_SMS_Browsable_Playable_Audio;
	

	class AudioPlot : public PlotBase
	{
	public:
		AudioPlot();
		AudioPlot( std::string name );
		virtual ~AudioPlot();

		void SetPosition( int x, int y );
		void SetSize( int w, int h );
		void SetLabel( const char* label );
		void SetYRange( double ymin, double ymax );
		void SetTooltipFormat( const char* tooltipFormat );
		
		void SetData( const CLAM::Audio& obj );

	protected:

		AudioAdapter*                      mpAdapter;
		Fl_SMS_Browsable_Playable_Audio*   mpWidget;
	};

}


#endif // AudioPlot.hxx
