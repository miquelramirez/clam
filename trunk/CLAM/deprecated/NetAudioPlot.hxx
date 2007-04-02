#ifndef __NETAUDIOPLOT__
#define __NETAUDIOPLOT__

#include "Audio.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	class AudioPortMonitor;

	namespace VM
	{
		class NetAudioPlot : public NetPlot
		{
			Q_OBJECT
		public:
			typedef AudioPortMonitor MonitorType;
			NetAudioPlot(QWidget* parent = 0, const char * name = 0);
			~NetAudioPlot();

			void SetData(const Audio& audio);
			void SetDataColor(Color c);
			void SetMonitor(MonitorType & monitor);

		protected:
			void SetPlotController();

		private:
			void InitNetAudioPlot();
		};
	}
}

#endif

