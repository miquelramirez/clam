#ifndef __NETSINTRACKSPLOTCONTROLLER__
#define __NETSINTRACKSPLOTCONTROLLER__

#include "SpectralPeakArray.hxx"
#include "NetSinTracksRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSinTracksPlotController : public NetPlotController 
	{
	public:
	    typedef SinTracksPortMonitor MonitorType;
	    NetSinTracksPlotController();
	    virtual ~NetSinTracksPlotController();

	    void SetData(const SpectralPeakArray& peaks);
	    void Draw();
	    void SetMonitor(MonitorType & monitor);

	protected:
	    void FullView();

	private:
	    MonitorType* mMonitor;
	    Array<SpectralPeakArray> _peakMtx;
	    NetSinTracksRenderer _renderer;
	    SigSlot::Slotv0 mSlotNewData;
	    int _index;

	    void AddData(const SpectralPeakArray& data);
	  
	    void Init();
	    void OnNewData();

	};
    }
}

#endif


