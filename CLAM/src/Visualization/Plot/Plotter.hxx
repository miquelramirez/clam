#ifndef __PLOTTER__
#define __PLOTTER__

#include <utility>
#include <list>

namespace CLAMVM
{
	class ModelAdapter;
	class Presentation;

	class Plotter
	{
	public:
		static void Hold( ModelAdapter* , Presentation* );
		static void PlotAll();
		static void Flush();
	protected:
		typedef std::pair< ModelAdapter*, Presentation* > tPlotCmd;
		typedef std::list< tPlotCmd > tPlotList;

		static tPlotList  mPendingPlots;
	};

}

#endif // Plotter.hxx
