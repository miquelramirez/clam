#ifndef __NETSPECGRAMPLOTCONTROLLER__
#define __NETSPECGRAMPLOTCONTROLLER__

#include <vector>
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
	    std::vector<Spectrum> _cachedData;
	    std::vector<Spectrum> _processedData;
	    NetSpecgramRenderer _renderer;
	    int _index;
	    
	    void AddData(const Spectrum& spec);
	    void AdaptSpectralData();
	    void ProcessData();
	  
	    void InitDataArrays();
	    void FullView();

	};
    }
}

#endif


