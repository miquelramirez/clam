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

		        void UpdatePoint(const TData& x, const TData& y);

	        protected:
		        void FullView();

		private:
			DataArray _magBuffer;
			DataArray _freqBuffer;
			
			NetPeaksRenderer _renderer;

			MonitorType * mMonitor;

		        bool _linear;
		        bool _hasData;
		        QString _tooltip;
		        bool _renderingIsDone;

			void ProcessPeakData();
		        void Init();
		};
	}
}

#endif

