#ifndef __NETAUDIOPLOTCONTROLLER__
#define __NETAUDIOPLOTCONTROLLER__

#include "Audio.hxx"
#include "DataRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetAudioPlotController : public NetPlotController
		{
		public:
			typedef AudioPortMonitor MonitorType;
			NetAudioPlotController();
			virtual ~NetAudioPlotController();

			void SetData(const Audio& audio);
			void SetDataColor(Color c);
			virtual void Draw();
			void SetMonitor(MonitorType & monitor);

		protected:
			void FullView();

		private:
			DataRenderer _dRenderer;

			MonitorType * mMonitor;
		};
	}
}

#endif

