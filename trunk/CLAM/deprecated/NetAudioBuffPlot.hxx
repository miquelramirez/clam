#ifndef __NETAUDIOBUFFPLOT__
#define __NETAUDIOBUFFPLOT__

#include "Audio.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	class AudioBuffPortMonitor;

	namespace VM
	{
		class NetAudioBuffPlot : public NetPlot
		{
			Q_OBJECT
		public:
			typedef AudioBuffPortMonitor MonitorType;
			NetAudioBuffPlot(QWidget* parent = 0, const char * name = 0);
			~NetAudioBuffPlot();

			void SetData(const Audio& audio);
			void SetDataColor(Color c);
			void SetMonitor(MonitorType & monitor);

		protected:
			void SetPlotController();

		private:
			void InitNetAudioBuffPlot();
		};
	}
}

#endif

