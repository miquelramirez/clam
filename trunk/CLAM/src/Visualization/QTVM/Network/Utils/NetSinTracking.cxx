#include "NetSinTracking.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSinTracking::NetSinTracking()
	    : _palette(0.0f), 
	      _time(0),
	      _leftIndex1(0),
	      _rightIndex1(0),
	      _leftIndex2(0),
	      _rightIndex2(0),
	      _begin(0)
	{
	}
	
	NetSinTracking::~NetSinTracking()
	{
	}

	std::vector< std::vector<SinTrackNode> >& NetSinTracking::GetTracks(const Array<SpectralPeakArray>& peakMtx )
	{
	    _table.clear();
	    _tracks.clear();
	    _time=_begin;
	    for(int i=_leftIndex1; i < _rightIndex1;i++)
	    {
		AddNodes(peakMtx[i]);
	    }
	    for(int i=_leftIndex2; i < _rightIndex2;i++)
	    {
		AddNodes(peakMtx[i]);
	    }
	    return _tracks;
	}

	void NetSinTracking::AddToTable(const TIndex& trackId, const unsigned& index)
	{
	    _table[trackId]=index;
	}
	
	bool NetSinTracking::HasTrackId(const TIndex& trackId)
	{
	    return (_table.find(trackId) != _table.end());
	}

	unsigned NetSinTracking::GetIndex(const TIndex& trackId)
	{
	    return _table[trackId];
	}

	float NetSinTracking::ClampToRange(TData value) const
	{
	    if ( value > 0.0 ) // 0 dB is the maximum
		return 1.0f;
	    if ( value < -100.0 ) // -100 dB is the minimum
		return 0.0f;

	    value += 100.0f;
	    value*= 0.01f;
	   
	    return value;
	}

	void NetSinTracking::AddNodes(const SpectralPeakArray& peaks)
	{
	    for(int i=0; i < peaks.GetMagBuffer().Size();i++)
	    {
		SinTrackNode node;
		node.freq=peaks.GetFreqBuffer()[i];
		node.timeIndex=_time;
		int colorIndex = _palette.Get( ClampToRange(peaks.GetMagBuffer()[i]) );
		_palette.GetRGBFromIndex( colorIndex, node.color.r, node.color.g, node.color.b);
		TIndex trackId = peaks.GetIndexArray()[i];
		if(HasTrackId(trackId))
		{
		    unsigned index=GetIndex(trackId);
		    _tracks[index].push_back(node);
		}
		else
		{
		    std::vector<SinTrackNode> newTrack;
		    newTrack.push_back(node);
		    _tracks.push_back(newTrack);
		    AddToTable(trackId,_tracks.size()-1);
		}
	    }
	    _time++;
	}
	
	void NetSinTracking::SetIndexes(const TIndex& left1, const TIndex& right1, 
					const TIndex& left2, const TIndex& right2, 
					const TIndex& begin)
	{
	    _leftIndex1=left1;
	    _rightIndex1=right1;
	    _leftIndex2=left2;
	    _rightIndex2=right2;
	    _begin=begin;
	}

    }
}

// END

