#ifndef __DPMULTIPLOT__
#define __DPMULTIPLOT__

#include <list>
#include <string>
#include "Array.hxx"
#include "BPF.hxx"
#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtMultiPlot;

		class DPMultiPlot
		{
			public:
				DPMultiPlot();
				virtual ~DPMultiPlot();
				
				void Label(const std::string& label);
				void Geometry(int x, int y, int w, int h);

				void AddData(const std::string& key, const DataArray& array);
				void AddData(std::string key, const BPF& data, int samples = 100);
				void RemoveData(std::string key );
				void RemoveAllData();

				void SetColor(std::string key, Color c);

				void SetMarks(std::list<unsigned>& marks);
				
				void SetXRange(const TData& xmin, const TData& xmax);
				void SetYRange(const TData& ymin, const TData& ymax);

				void SetUnits(const std::string& xunits, const std::string& yunits);
				void SetToolTips(const std::string& xtooltip, const std::string& ytooltip);

				void Run();

			private:
				QtMultiPlot* _plot;
				
		};
	}
}

#endif
