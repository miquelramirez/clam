#ifndef __NETFUNDTRACKPLOT__
#define __NETFUNDTRACKPLOT__

#include "Fundamental.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetFundTrackPlot : public NetPlot
		{
			Q_OBJECT
		public:
			NetFundTrackPlot(QWidget* parent = 0, const char * name = 0);
			virtual ~NetFundTrackPlot();

			void SetData(const Fundamental& fund);
			void SetDataColor(Color c);

		protected:
			virtual void SetPlotController();

		private:
			void InitNetFundTrackPlot();
		};
	}
}

#endif

