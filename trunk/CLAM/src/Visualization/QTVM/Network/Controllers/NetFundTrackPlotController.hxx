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

	    void UpdatePoint(const TData& x, const TData& y);

	protected:
	    void FullView();

	private:
	    DataArray _cachedData;
	    BuffDataRenderer _renderer;
	    int _index;
	    SigSlot::Slotv0 mSlotNewData;

	    MonitorType* mMonitor;
	    
	    bool _hasData;
	    QString _tooltip;
	    bool _renderingIsDone;
	    
	    void AddData(const TData& data);
	  
	    void Init();
	    void OnNewData();

	};
    }
}

#endif


