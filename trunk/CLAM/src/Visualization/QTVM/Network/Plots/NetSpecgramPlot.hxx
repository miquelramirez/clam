#ifndef __NETSPECGRAMPLOT__
#define __NETSPECGRAMPLOT__

#include "Spectrum.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSpecgramPlot : public NetPlot
	{
	    Q_OBJECT
	public:
	    NetSpecgramPlot(QWidget* parent = 0, const char * name = 0);
	    virtual ~NetSpecgramPlot();

	    void SetData(const Spectrum& spec);

	protected:
	    virtual void SetPlotController();

	private:
	    void InitNetSpecgramPlot();
	};
    }
}

#endif

