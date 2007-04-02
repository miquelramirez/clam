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
			~NetFundPlotController();

			void SetData(const Fundamental& data);
			void SetDataColor(Color c);
			void Draw();
			void SetMonitor(MonitorType & monitor);

			void UpdatePoint(const TData& x, const TData& y);

		protected:
			void FullView();

		private:
			MonitorType* mMonitor;
			Fundamental  mFund;
			Dial         mRenderer;    
			bool         mHasData;
			bool         mRenderingIsDone;
			QString      mTooltip;
		        
			void Init();
		};
	}
}

#endif

