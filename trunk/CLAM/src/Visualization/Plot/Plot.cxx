#include "Plot.hxx"
#include "Plotter.hxx"

namespace CLAMVM
{
	void showPendingPlots( )
	{
		Plotter::PlotAll();
		Plotter::Flush();
	}
}
