#ifndef __NETSPECGRAMPLOTCONTROLLER__
#define __NETSPECGRAMPLOTCONTROLLER__

#include "Array.hxx"
#include "Spectrum.hxx"
#include "NetSpecgramRenderer.hxx"
#include "NetPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSpecgramPlotController : public NetPlotController 
	{
	public:
	    NetSpecgramPlotController();
	    virtual ~NetSpecgramPlotController();

	    void SetData(const Spectrum& spec);
	    void Draw();

	private:
	    Spectrum _spec;
	    Array<DataArray> _cachedData;
	    Array<DataArray> _processedData;
	    NetSpecgramRenderer _renderer;
	    int _index;
	    int _specSize;
	    bool _first;
	    
	    void AddData(const Spectrum& spec);
	    void AdaptSpectralData();
	    void ProcessData();
	  
	    void FullView();
	    void Init(const TSize& specSize);

	};
    }
}

#endif


