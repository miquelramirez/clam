#ifndef __NETSINTRACKSPLOTCONTROLLER__
#define __NETSINTRACKSPLOTCONTROLLER__

#include "SpectralPeakArray.hxx"
#include "NetSinTracksRenderer.hxx"
#include "NetPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSinTracksPlotController : public NetPlotController 
	{
	public:
	    NetSinTracksPlotController();
	    virtual ~NetSinTracksPlotController();

	    void SetData(const SpectralPeakArray& peaks);
	    void Draw();

	private:
	    Array<SpectralPeakArray> _peakMtx;
	    SineTrackList _cachedTracks;
	    NetSinTracksRenderer _renderer;
	    TSize _peakMtxOldSize;
	    
	    void AddData(const SpectralPeakArray& data);
	    void ProcessData();
	  
	    void InitPeakMatrix();
	    void InitView();
	    void CheckView();

	};
    }
}

#endif


