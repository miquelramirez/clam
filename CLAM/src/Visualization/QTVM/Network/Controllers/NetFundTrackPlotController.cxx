#include <algorithm>
#include "NetFundTrackPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetFundTrackPlotController::NetFundTrackPlotController()
	{
	    InitDataArrays();
	    SetvRange(TData(0.0),TData(2050.0));
	    SetnSamples(200);
	    InitView();
	}

	NetFundTrackPlotController::~NetFundTrackPlotController()
	{
	}

	void NetFundTrackPlotController::SetData(const Fundamental& fund)
	{
	    AddData(fund.GetFreq(0));
	    ProcessData();
	    emit sendView(_view);
	}

	void NetFundTrackPlotController::SetDataColor(Color c)
	{
	    _renderer.SetColor(c);
	}

	void NetFundTrackPlotController::Draw()
	{
	    _renderer.Render();
	}

	void NetFundTrackPlotController::AddData(const TData& data)
	{
	    _cachedData.AddElem(data);
	}

	void NetFundTrackPlotController::ProcessData()
	{
	    TSize offset, len;

	    if(_cachedData.Size() <= GetnSamples())
	    {
		offset = 0;
		len = _cachedData.Size();
	    }
	    else
	    {
		offset = _cachedData.Size()-GetnSamples();
		len = _cachedData.Size()-offset;
	    }

	    if(_processedData.Size() < len)
		_processedData.Resize(len);
	    _processedData.SetSize(len);

	    std::copy(_cachedData.GetPtr()+offset, _cachedData.GetPtr()+offset+len, _processedData.GetPtr());
	    _renderer.SetDataPtr(_processedData.GetPtr(),_processedData.Size(),NormalMode);
	}

	void NetFundTrackPlotController::InitDataArrays()
	{
	    _cachedData.Init();
	    _processedData.Init();
	}

	void NetFundTrackPlotController::InitView()
	{
	    _view.left=TData(0.0);
	    _view.right=TData(GetnSamples());
	    _view.bottom=GetvMin();
	    _view.top=GetvMax();
	    emit sendView(_view);
	}

    }
}

// END


