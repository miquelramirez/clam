#ifndef __NETAUDIOBUFFPLOTCONTROLLER__
#define __NETAUDIOBUFFPLOTCONTROLLER__

#include "Audio.hxx"
#include "BuffDataRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetAudioBuffPlotController : public NetPlotController 
	{
	public:
	    typedef AudioBuffPortMonitor MonitorType;
	    NetAudioBuffPlotController();
	    virtual ~NetAudioBuffPlotController();

	    void SetData(const Audio& audio);
	    void SetDataColor(Color c);
	    void Draw();
	    void SetMonitor(MonitorType & monitor);
	
	protected:
	    void FullView();

	private:
	    MonitorType* mMonitor;
	    DataArray _cachedData;
	    BuffDataRenderer _renderer;
	    int _index;
	    TSize _frameSize;
	    SigSlot::Slotv0 mSlotNewData;

	    void AddData(const DataArray& data);
	  
	    void Init(const TSize& frameSize);
	    void OnNewData();
	    
	};
    }
}

#endif


