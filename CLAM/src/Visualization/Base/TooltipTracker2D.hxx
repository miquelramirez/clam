#ifndef __TOOLTIPTRACKER2D__
#define __TOOLTIPTRACKER2D__

#include "Slotv4.hxx"

namespace CLAMVM
{

	class DrawingSurface2D;

	class TooltipTracker2D
	{
	public:
		TooltipTracker2D();
		virtual ~TooltipTracker2D();

		void Track( DrawingSurface2D* surf );
		bool HandleMotion( int x, int y );
		void ForceText( const char* text );
		
		SigSlot::Slotv4< int, int, char*, int >  RenderTooltipText;

	protected:

		void DefaultTooltipTextRender( int x, int y, char* text, int maxLength );

		char               mTextBuffer[256];
		DrawingSurface2D*  mTrackedSurface;
	};

}

#endif // TooltipTracker2D.hxx
