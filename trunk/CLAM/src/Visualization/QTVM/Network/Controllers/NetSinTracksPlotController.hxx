#ifndef __NETSINTRACKSPLOTCONTROLLER__
#define __NETSINTRACKSPLOTCONTROLLER__

#include "SpectralPeakArray.hxx"
#include "NetSinTracksRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"
#include "SinTracking.hxx"

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

	    void UpdatePoint(const TData& x, const TData& y);

	protected:
	    void FullView();

	private:
	    MonitorType* mMonitor;
	    Array<SpectralPeakArray> _peakMtx;
	    NetSinTracksRenderer _renderer;
	    SigSlot::Slotv0 mSlotNewData;
	    int _index;
	    SinTracking mSinTracking;
	    TIndex _leftIndex1,_rightIndex1,_leftIndex2,_rightIndex2;
	    bool _hasData;
	    QString _tooltip;
	    bool _renderingIsDone;

	    void AddData(const SpectralPeakArray& data);
	  
	    void Init();
	    void OnNewData();
	    void ComputeIndexes();

	};
    }
}

#endif


