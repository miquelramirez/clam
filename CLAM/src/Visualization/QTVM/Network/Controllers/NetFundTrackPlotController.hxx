#ifndef __NETFUNDTRACKPLOTCONTROLLER__
#define __NETFUNDTRACKPLOTCONTROLLER__

#include "Fundamental.hxx"
#include "DataRenderer.hxx"
#include "NetPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetFundTrackPlotController : public NetPlotController 
	{
	public:
	    NetFundTrackPlotController();
	    virtual ~NetFundTrackPlotController();

	    void SetData(const Fundamental& fund);
	    void SetDataColor(Color c);
	    void Draw();

	private:
	    DataArray _cachedData;
	    DataArray _processedData;
	    DataRenderer _renderer;
	    
	    void AddData(const TData& data);
	    void ProcessData();
	  
	    void InitDataArrays();
	    void InitView();

	};
    }
}

#endif


