#ifndef __NETSINTRACKSPLOT__
#define __NETSINTRACKSPLOT__

#include "SpectralPeakArray.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	class SinTracksPortMonitor;

	namespace VM
	{
		class NetSinTracksPlot : public NetPlot
		{
			Q_OBJECT
		public:
			typedef SinTracksPortMonitor MonitorType;
			NetSinTracksPlot(QWidget* parent = 0, const char * name = 0);
			~NetSinTracksPlot();

			void SetData(const SpectralPeakArray& peaks);
			void SetMonitor(MonitorType & monitor);

		protected:
			void SetPlotController();

		private:
			void InitNetSinTracksPlot();
		};
	}
}

#endif

