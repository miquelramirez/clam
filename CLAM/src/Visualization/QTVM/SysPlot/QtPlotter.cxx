#include "QtAppWrapper.hxx"
#include "QtPlot.hxx"
#include "QtPlotter.hxx"

namespace CLAM
{
	namespace VM
	{
		QtPlotter::PlotList QtPlotter::_pList;

		void QtPlotter::Add(QtPlot* plot)
		{
			_pList.push_back(plot);
		}

		void QtPlotter::Remove(QtPlot* plot)
		{
			PlotList::iterator it = _pList.begin();
			for(;it != _pList.end();it++)
			{
				if((*it)==plot)
				{
					delete (*it);
					_pList.erase(it);
					break;
				}
			}
		}

		void QtPlotter::ShowAll()
		{
			PlotList::iterator it = _pList.begin();	
			for(; it!= _pList.end();it++) (*it)->Show();
			QtAppWrapper::Run();
		}
	}
}

// END

