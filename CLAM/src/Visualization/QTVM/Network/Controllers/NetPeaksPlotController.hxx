#ifndef __NETPEAKSPLOTCONTROLLER__
#define __NETPEAKSPLOTCONTROLLER__

#include "SpectralPeakArray.hxx"
#include "NetPeaksRenderer.hxx"
#include "NetPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
	    class NetPeaksPlotController : public NetPlotController
	    {
	    public:
		NetPeaksPlotController();
		virtual ~NetPeaksPlotController();

		void SetData(const SpectralPeakArray& peaks);
		void SetPeaksColor(Color cline, Color cpoint);
		void Draw();

	    protected:
		void FullView();

	    private:
		DataArray _magBuffer;
		DataArray _freqBuffer;

		NetPeaksRenderer _renderer;

		bool _linear;

		void ProcessPeakData();
	    };
	    
	}
}

#endif


