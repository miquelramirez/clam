#include "SineTracksAdapter.hxx"
#include "NetSinTracksPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSinTracksPlotController::NetSinTracksPlotController()
	{
	    InitPeakMatrix();
	    SetnSamples(200);
	    SetvRange(TData(0.0),TData(11025.0));
	    InitView();
	}

	NetSinTracksPlotController::~NetSinTracksPlotController()
	{
	}
	
	void NetSinTracksPlotController::SetData(const SpectralPeakArray& peaks)
	{
	    AddData(peaks);
	    CheckView();
	    SineTracksAdapter _adapter;
	    _cachedTracks = _adapter.GetTrackList(_peakMtx);
	    ProcessData();
	    emit sendView(_view);
	}

	void NetSinTracksPlotController::Draw()
	{
	    _renderer.Render();
	}

	void NetSinTracksPlotController::AddData(const SpectralPeakArray& data)
	{
	    _peakMtxOldSize=_peakMtx.Size();
	    if(!data.GetMagBuffer().Size()) return;
	    _peakMtx.AddElem(data);
	}

	void NetSinTracksPlotController::ProcessData()
	{
	    _renderer.SetHBounds(_view.left,_view.right);
	    _renderer.SetData(_cachedTracks);
	}

	void NetSinTracksPlotController::CheckView()
	{
	    TSize newSize = _peakMtx.Size();
	    if(_peakMtxOldSize == newSize) return;
	    if(_peakMtx.Size() > GetnSamples())
	    {
		_view.left = _view.left+TData(1.0);
		_view.right = _view.right+TData(1.0);
	    }
	}

	void NetSinTracksPlotController::InitPeakMatrix()
	{
	    _peakMtx.Init();
	}

	void NetSinTracksPlotController::InitView()
	{
	    _view.left=TData(0.0);
	    _view.right=TData(GetnSamples());
	    _view.top = GetvMax();
	    _view.bottom = GetvMin();
	}

    }
}

// END

