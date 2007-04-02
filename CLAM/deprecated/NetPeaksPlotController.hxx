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
			~NetPeaksPlotController();

			void SetData(const SpectralPeakArray& peaks);
			void SetPeaksColor(Color cline, Color cpoint);
			void Draw();
			void SetMonitor(MonitorType & monitor);

			void UpdatePoint(const TData& x, const TData& y);

		protected:
			void FullView();

		private:
			MonitorType*     mMonitor;
			DataArray        mMagBuffer;
			DataArray        mFreqBuffer;
			NetPeaksRenderer mRenderer;
			bool             mLinear;
			bool             mHasData;
			bool             mRenderingIsDone;
			QString          mTooltip;

			void ProcessPeakData();
			void Init();
		};
	}
}

#endif

