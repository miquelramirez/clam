#include "NetFundTrackPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetFundTrackPlotController::NetFundTrackPlotController()
	    : _index(0)
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
	    if(_cachedData.Size() < GetnSamples())
	    {
		_cachedData.AddElem(data);
	    }
	    else
	    {
		_cachedData[_index++] = data;
		if(_index == GetnSamples()) _index = 0;
	    }
	}

	void NetFundTrackPlotController::ProcessData()
	{
	    if(_cachedData.Size() < GetnSamples())
	    {
		_renderer.SetDataPtr(_cachedData.GetPtr(),_cachedData.Size(),NormalMode);
	    }
	    else
	    {
		int i,j = 0;
		for(i = _index; i < _cachedData.Size(); i++)
		    _processedData[j++]=_cachedData[i];
		for(i = 0; i < _index; i++)
		    _processedData[j++] = _cachedData[i];
		 _renderer.SetDataPtr(_processedData.GetPtr(),_processedData.Size(),NormalMode);
	    }
	}

	void NetFundTrackPlotController::InitDataArrays()
	{
	    _cachedData.Init();
	    _processedData.Resize(200);
	    _processedData.SetSize(200);
	}

	void NetFundTrackPlotController::InitView()
	{
	    _view.left=TData(0.0);
	    _view.right=TData(GetnSamples());
	    _view.bottom=GetvMin();
	    _view.top=GetvMax();
	}

    }
}

// END


