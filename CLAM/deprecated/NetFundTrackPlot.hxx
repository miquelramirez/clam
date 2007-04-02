#ifndef __NETFUNDTRACKPLOT__
#define __NETFUNDTRACKPLOT__

#include "Fundamental.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	class FundTrackPortMonitor;

	namespace VM
	{
		class NetFundTrackPlot : public NetPlot
		{
			Q_OBJECT
		public:
			typedef FundTrackPortMonitor MonitorType;
			NetFundTrackPlot(QWidget* parent = 0, const char * name = 0);
			~NetFundTrackPlot();

			void SetData(const Fundamental& fund);
			void SetDataColor(Color c);
			
			void SetMonitor(MonitorType & monitor);

		protected:
			void SetPlotController();

		private:
			void InitNetFundTrackPlot();
		};
	}
}

#endif

