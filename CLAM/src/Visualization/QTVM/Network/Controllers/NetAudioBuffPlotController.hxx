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

	    void UpdatePoint(const TData& x, const TData& y);
	
	protected:
	    void FullView();

	private:
	    MonitorType* mMonitor;
	    DataArray _cachedData;
	    BuffDataRenderer _renderer;
	    int _index;
	    TSize _frameSize;
	    TIndex _leftIndex1,_rightIndex1,_leftIndex2,_rightIndex2;
	    SigSlot::Slotv0 mSlotNewData;
	    bool _hasData;
	    QString _tooltip;
	    bool _renderingIsDone;

	    void AddData(const DataArray& data);
	  
	    void Init(const TSize& frameSize);
	    void OnNewData();
	    void ComputeIndexes();
	};
    }
}

#endif


