#ifndef __NETFUNDPLOTCONTROLLER__
#define __NETFUNDPLOTCONTROLLER__

#include "Fundamental.hxx"
#include "Dial.hxx"
#include "NetPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetFundPlotController : public NetPlotController
		{
			public:
				NetFundPlotController();
				virtual ~NetFundPlotController();

				void SetData(const Fundamental& data);
				void SetDataColor(Color c);
				void Draw();

			protected:
				void FullView();
				
			private:
				Dial _renderer;

		};
	}
}

#endif

