#ifndef __NETFUNDPLOT__
#define __NETFUNDPLOT__

#include "Fundamental.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	class FundamentalPortMonitor;

	namespace VM
	{
		class NetFundPlot : public NetPlot
		{
			Q_OBJECT
		public:
			typedef FundamentalPortMonitor MonitorType;
			NetFundPlot(QWidget* parent = 0, const char * name = 0);
			~NetFundPlot();

			void SetData(const Fundamental& spec);
			void SetDataColor(Color c);
			void SetMonitor(MonitorType & monitor);

		protected:
			void SetPlotController();

		private:
			void InitNetFundPlot();
		};
	}
}

#endif

