#ifndef __NETFUNDTRACKPLOTCONTROLLER__
#define __NETFUNDTRACKPLOTCONTROLLER__

#include "Fundamental.hxx"
#include "BuffDataRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetFundTrackPlotController : public NetPlotController 
	{
	public:
	    typedef FundTrackPortMonitor MonitorType;
	    NetFundTrackPlotController();
	    virtual ~NetFundTrackPlotController();

	    void SetData(const Fundamental& fund);
	    void SetDataColor(Color c);
	    void Draw();
	    void SetMonitor(MonitorType & monitor);

	private:
	    DataArray _cachedData;
	    BuffDataRenderer _renderer;
	    int _index;
	    SigSlot::Slotv0 mSlotNewData;

	    MonitorType* mMonitor;
	    
	    void AddData(const TData& data);
	  
	    void Init();
	    void FullView();
	    void OnNewData();

	};
    }
}

#endif


