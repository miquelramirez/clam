#ifndef __NETAUDIOBUFFPLOT__
#define __NETAUDIOBUFFPLOT__

#include "Audio.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetAudioBuffPlot : public NetPlot
		{
			Q_OBJECT
		public:
			NetAudioBuffPlot(QWidget* parent = 0, const char * name = 0);
			virtual ~NetAudioBuffPlot();

			void SetData(const Audio& audio);
			void SetDataColor(Color c);

		protected:
			virtual void SetPlotController();

		private:
			void InitNetAudioBuffPlot();
		};
	}
}

#endif

