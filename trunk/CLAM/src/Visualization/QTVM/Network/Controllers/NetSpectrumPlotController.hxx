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

		        void UpdatePoint(const TData& x, const TData& y);

		protected:
		        void FullView();

		private:
			Spectrum  _spec;

			DataRenderer _renderer;

			MonitorType * mMonitor;
		       
		        TData _spectralRange;
		        bool _hasData;
		        QString _tooltip;
		        bool _renderingIsDone;
		    

			void AdaptSpectralData();
		        void Init(const TSize& size);
		};
	}
}

#endif

