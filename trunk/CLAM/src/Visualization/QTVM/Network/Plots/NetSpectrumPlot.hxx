#ifndef __NETSPECTRUMPLOT__
#define __NETSPECTRUMPLOT__

#include "Spectrum.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetSpectrumPlot : public NetPlot
		{
			public:
				NetSpectrumPlot(QWidget* parent=0);
				virtual ~NetSpectrumPlot();

				void SetData(const Spectrum& spec);
				void SetDataColor(Color c);

			protected:
				virtual void SetPlotController();

			private:
				void InitNetSpectrumPlot();
		};
	}
}

#endif

