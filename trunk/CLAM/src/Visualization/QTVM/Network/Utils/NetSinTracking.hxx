#ifndef __NETSINTRACKING__
#define __NETSINTRACKING__

#include <map>
#include <vector>
#include "SpectralPeakArray.hxx"
#include "SinTrackNode.hxx"
#include "QtPalette.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSinTracking
	{
	public:
	    NetSinTracking();
	    ~NetSinTracking();

	    void SetIndexes(const TIndex& left1, const TIndex& right1, 
			    const TIndex& left2, const TIndex& right2, 
			    const TIndex& begin);
	    std::vector< std::vector<SinTrackNode> >& GetTracks(const Array<SpectralPeakArray>& peakMtx);

	private:
	    std::map<TIndex,unsigned> _table;
	    std::vector< std::vector<SinTrackNode> > _tracks;
	    QtPalette _palette;
	    TIndex _time;
	    TIndex _leftIndex1,_rightIndex1,_leftIndex2,_rightIndex2;
	    TIndex _begin;

	    void AddToTable(const TIndex& trackId, const unsigned& index);
	    bool HasTrackId(const TIndex& trackId);
	    unsigned GetIndex(const TIndex& trackId);

	    void AddNodes(const SpectralPeakArray& peaks);

	    float ClampToRange(TData value) const;
	};
    }
}

#endif

