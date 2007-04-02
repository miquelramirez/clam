#ifndef __NETSPECTRUMPLOTCONTROLLER__
#define __NETSPECTRUMPLOTCONTROLLER__

#include "Spectrum.hxx"
#include "NetDataRenderer.hxx"
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
			~NetSpectrumPlotController();

			void SetData(const Spectrum& spec);
			void SetDataColor(Color c);
			void Draw();
			void SetMonitor(MonitorType & monitor);

			void UpdatePoint(const TData& x, const TData& y);

		protected:
			void FullView();

		private:
			Spectrum        mSpec;
			NetDataRenderer mRenderer;
			MonitorType*    mMonitor;
			TData           mSpectralRange;
			bool            mHasData;    
			bool            mRenderingIsDone;
			QString         mTooltip;
		    
			void AdaptSpectralData();
			void Init(const TSize& size);
		};
	}
}

#endif

