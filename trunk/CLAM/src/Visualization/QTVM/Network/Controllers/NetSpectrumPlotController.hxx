#ifndef __NETSPECTRUMPLOTCONTROLLER__
#define __NETSPECTRUMPLOTCONTROLLER__

#include "Spectrum.hxx"
#include "DataRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetSpectrumPlotController : public NetPlotController
		{
		public:
			typedef SpectrumPortMonitor MonitorType;
			NetSpectrumPlotController();
			virtual ~NetSpectrumPlotController();

			void SetData(const Spectrum& spec);
			void SetDataColor(Color c);
			virtual void Draw();
			void SetMonitor(MonitorType & monitor);

		private:
			Spectrum  _spec;

			DataRenderer _renderer;

			MonitorType * mMonitor;

			void AdaptSpectralData();
		        void Init(const TSize& size);
		        void FullView();
		};
	}
}

#endif

