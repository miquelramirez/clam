#ifndef __NETPEAKSPLOTCONTROLLER__
#define __NETPEAKSPLOTCONTROLLER__

#include "SpectralPeakArray.hxx"
#include "NetPeaksRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetPeaksPlotController : public NetPlotController
		{
		public:
			typedef PeaksPortMonitor MonitorType;
			NetPeaksPlotController();
			virtual ~NetPeaksPlotController();

			void SetData(const SpectralPeakArray& peaks);
			void SetPeaksColor(Color cline, Color cpoint);
			virtual void Draw();
			void SetMonitor(MonitorType & monitor);

		private:
			DataArray _magBuffer;
			DataArray _freqBuffer;
			bool _linear;

			NetPeaksRenderer _renderer;

			MonitorType * mMonitor;

			void ProcessPeakData();
		        void Init();
		        void FullView();
		};
	}
}

#endif

