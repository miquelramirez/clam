#ifndef __NETSINTRACKSPLOT__
#define __NETSINTRACKSPLOT__

#include "SpectralPeakArray.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetSinTracksPlot : public NetPlot
		{
			Q_OBJECT
		public:
			NetSinTracksPlot(QWidget* parent = 0, const char * name = 0);
			virtual ~NetSinTracksPlot();

			void SetData(const SpectralPeakArray& peaks);

		protected:
			virtual void SetPlotController();

		private:
			void InitNetSinTracksPlot();
		};
	}
}

#endif

