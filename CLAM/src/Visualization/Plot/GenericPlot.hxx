#ifndef __GENERICPLOT__
#define __GENERICPLOT__

#include "PlotBase.hxx"
#include <string>

namespace CLAMVM
{
	class Fl_Plot;
	
	class GenericPlot : public PlotBase
	{

	public:
		virtual ~GenericPlot();

		virtual void SetPosition( int x, int y );
		virtual void SetSize( int w, int h );
		virtual void SetLabel( const char* label );
		virtual void SetYRange( double ymin, double ymax );
		virtual void SetTooltipFormat( const char* tooltipFmt );
		
	protected:
		void RegisterAnonymously();
		void RegisterAs( std::string name );

		Fl_Plot* GetWidget();
		void     SetWidget( Fl_Plot* );

		virtual void CreateWidget();
		virtual  void DestroyWidget();
	private:
		Fl_Plot*    mWidget;
	};

	// Inline definitions
	
	inline Fl_Plot* GenericPlot::GetWidget() {
		return mWidget;
	}

	inline void GenericPlot::SetWidget( Fl_Plot* w) {
		mWidget = w;
	}

}

#endif // GenericPlot.hxx
