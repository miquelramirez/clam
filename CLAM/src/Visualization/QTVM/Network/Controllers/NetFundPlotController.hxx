#ifndef __NETFUNDPLOTCONTROLLER__
#define __NETFUNDPLOTCONTROLLER__

#include "Fundamental.hxx"
#include "Dial.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetFundPlotController : public NetPlotController
		{
		public:
			typedef FundamentalPortMonitor MonitorType;
			NetFundPlotController();
			virtual ~NetFundPlotController();

			void SetData(const Fundamental& data);
			void SetDataColor(Color c);
			void Draw();
			void SetMonitor(MonitorType & monitor);

		private:
		        Fundamental _fund;
			Dial _renderer;

			MonitorType * mMonitor;

		        void Init();
		        void FullView();
		};
	}
}

#endif

