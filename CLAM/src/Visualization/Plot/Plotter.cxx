#include "Plotter.hxx"
#include "ModelAdapter.hxx"
#include "Presentation.hxx"
#include "WidgetTKWrapper.hxx"
#include "Assert.hxx"

namespace CLAMVM
{
	Plotter::tPlotList Plotter::mPendingPlots;

	void Plotter::Hold( ModelAdapter* adap, Presentation* widget )
	{
		mPendingPlots.push_back( tPlotCmd( adap, widget) );
	}

	void Plotter::PlotAll( )
	{
		tPlotList::iterator i = mPendingPlots.begin();
		
		while ( i!= mPendingPlots.end() )
		{
			i->second->Show();
			i++;
		}

		// Run widget toolkit wrapper:
		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();

	}

	void Plotter::Flush()
	{
		tPlotList::iterator i = mPendingPlots.begin();
		
		while ( i!= mPendingPlots.end() )
		{
			CLAM_ASSERT( i->first !=NULL, "Pointer was NULL!" );
			CLAM_ASSERT( i->second !=NULL, "Pointer was NULL!" );
			delete i->first;
			delete i->second;
			i++;
		}

		mPendingPlots.clear();
		
	}
}
