#ifndef __NETSPECTRUMPLOTCONTROLLER__
#define __NETSPECTRUMPLOTCONTROLLER__
#include "Spectrum.hxx"
#include "DataRenderer.hxx"
#include "NetPlotController.hxx"
namespace CLAM
{
	namespace VM
	{
		class NetSpectrumPlotController : public NetPlotController
		{
			public:
				NetSpectrumPlotController();
				virtual ~NetSpectrumPlotController();
				void SetData(const Spectrum& spec);
				void SetDataColor(Color c);

				virtual void Draw();
			protected:
				void FullView();
			private:
				Spectrum  _spec;
				DataRenderer _renderer;
				DataArray _magBuffer;

				bool _mustProcessData;
				void AdaptSpectralData();
				void CacheData();
				void ProcessData();
		};
	}
}

#endif




