#ifndef __QTPLOTTER__
#define __QTPLOTTER__

#include <list>

namespace CLAM
{
	namespace VM
	{
		class QtPlot;
		
		class QtPlotter
		{
			typedef std::list<QtPlot*> PlotList;

			public:
				static void Add(QtPlot* plot);
				static void Remove(QtPlot* plot);
				static void ShowAll();
				
			private:				
				static PlotList _pList;

		};
	}
}

#endif

