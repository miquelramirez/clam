#ifndef __NETSPECGRAMPLOTCONTROLLER__
#define __NETSPECGRAMPLOTCONTROLLER__

#include <vector>
#include "Spectrum.hxx"
#include "QtPalette.hxx"
#include "NetSpecgramRenderer.hxx"
#include "NetPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSpecgramPlotController : public NetPlotController 
	{
	public:
	    typedef SpecgramPortMonitor MonitorType;
	    NetSpecgramPlotController();
	    virtual ~NetSpecgramPlotController();

	    void SetData(const Spectrum& spec);
	    void Draw();
	    void SetMonitor(MonitorType & monitor);

	protected:
	    void FullView();

	private:
	    MonitorType* mMonitor;
	    Spectrum _spec;
	    std::vector< std::vector<Color> > _cachedData;
	    NetSpecgramRenderer _renderer;
	    int _index;
	    int _specSize;
	    QtPalette _palette;
	    SigSlot::Slotv0 mSlotNewData;
	    TIndex _leftIndex1, _rightIndex1, _leftIndex2, _rightIndex2;
	    TIndex _bottomBound, _topBound;
	    
	    void AddData(const Spectrum& spec);
	    void AdaptSpectralData();
	  
	    void Init(const TSize& specSize);

	    float ClampToRange(TData value) const;

	    void OnNewData();
	    void ComputeIndexes();

	};
    }
}

#endif


