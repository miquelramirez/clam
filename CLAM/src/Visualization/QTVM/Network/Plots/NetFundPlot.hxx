#ifndef __NETFUNDPLOT__
#define __NETFUNDPLOT__

#include "Fundamental.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetFundPlot : public NetPlot
		{
			Q_OBJECT
		public:
			NetFundPlot(QWidget* parent = 0, const char * name = 0);
			virtual ~NetFundPlot();

			void SetData(const Fundamental& spec);
			void SetDataColor(Color c);

		protected:
			virtual void SetPlotController();

		private:
			void InitNetFundPlot();
		};
	}
}

#endif

