#ifndef __NETPEAKSPLOT__
#define __NETPEAKSPLOT__

#include "SpectralPeakArray.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetPeaksPlot : public NetPlot
	{
	public:
	    NetPeaksPlot(QWidget* parent=0);
	    virtual ~NetPeaksPlot();

	    void SetData(const SpectralPeakArray& peaks);
	    void SetPeaksColor(Color cline, Color cpoint);

	protected:
	    virtual void SetPlotController();

	private:
	    void InitNetPeaksPlot();
	};
    }
}

#endif

