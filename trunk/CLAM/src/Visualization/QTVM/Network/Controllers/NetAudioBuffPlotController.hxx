#ifndef __NETAUDIOBUFFPLOTCONTROLLER__
#define __NETAUDIOBUFFPLOTCONTROLLER__

#include "Audio.hxx"
#include "DataRenderer.hxx"
#include "NetPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetAudioBuffPlotController : public NetPlotController 
	{
	public:
	    NetAudioBuffPlotController();
	    virtual ~NetAudioBuffPlotController();

	    void SetData(const Audio& audio);
	    void SetDataColor(Color c);
	    void Draw();

	private:
	    DataArray _cachedData;
	    DataArray _processedData;
	    DataRenderer _renderer;
	    int _index;
	    bool _first;
	    TSize _frameSize;
	    
	    void AddData(const DataArray& data);
	    void ProcessData();
	  
	    void Init(const TSize& frameSize);
	    void FullView();

	};
    }
}

#endif


