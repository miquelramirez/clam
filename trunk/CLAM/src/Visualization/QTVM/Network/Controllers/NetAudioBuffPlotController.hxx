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

	private:
	    MonitorType* mMonitor;
	    DataArray _cachedData;
	    BuffDataRenderer _renderer;
	    int _index;
	    TSize _frameSize;
	    
	    void AddData(const DataArray& data);
	  
	    void Init(const TSize& frameSize);
	    void FullView();
	    
	};
    }
}

#endif


