#ifndef __NETSPECGRAMPLOTCONTROLLER__
#define __NETSPECGRAMPLOTCONTROLLER__

#include <vector>
#include "Spectrum.hxx"
#include "QtPalette.hxx"
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
	    std::vector< std::vector<Color> > _cachedData;
	    NetSpecgramRenderer _renderer;
	    int _index;
	    int _specSize;
	    bool _first;
	    QtPalette _palette;
	    
	    void AddData(const Spectrum& spec);
	    void AdaptSpectralData();
	  
	    void FullView();
	    void Init(const TSize& specSize);

	    float ClampToRange(TData value) const;

	};
    }
}

#endif


