#ifndef __NETAUDIOPLOT__
#define __NETAUDIOPLOT__

#include "Audio.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetAudioPlot : public NetPlot
		{
			Q_OBJECT
			public:
				NetAudioPlot(QWidget* parent=0);
				virtual ~NetAudioPlot();

				void SetData(const Audio& audio);
				void SetDataColor(Color c);

			protected:
				virtual void SetPlotController();

			private:
				void InitNetAudioPlot();
		};
	}
}

#endif

