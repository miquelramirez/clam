#include "NetSinTracking.hxx"

namespace CLAM
{
    namespace VM
    {
		NetSinTracking::NetSinTracking()
			: mPalette(0.0f)
			, mTime(0)
			, mLeftIndex1(0)
			, mRightIndex1(0)
			, mLeftIndex2(0)
			, mRightIndex2(0)
			, mBegin(0)
		{
		}
	
		NetSinTracking::~NetSinTracking()
		{
		}

		std::vector< std::vector<SinTrackNode> >& NetSinTracking::GetTracks(const Array<SpectralPeakArray>& peakMtx )
		{
			mTable.clear();
			mTracks.clear();
			mTime=mBegin;
			for(int i=mLeftIndex1; i < mRightIndex1;i++)
			{
				AddNodes(peakMtx[i]);
			}
			for(int i=mLeftIndex2; i < mRightIndex2;i++)
			{
				AddNodes(peakMtx[i]);
			}
			return mTracks;
		}

		void NetSinTracking::AddToTable(const TIndex& trackId, const unsigned& index)
		{
			mTable[trackId]=index;
		}
	
		bool NetSinTracking::HasTrackId(const TIndex& trackId)
		{
			return (mTable.find(trackId) != mTable.end());
		}

		unsigned NetSinTracking::GetIndex(const TIndex& trackId)
		{
			return mTable[trackId];
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
				node.timeIndex=mTime;
				int colorIndex = mPalette.Get( ClampToRange(peaks.GetMagBuffer()[i]) );
				mPalette.GetRGBFromIndex( colorIndex, node.color.r, node.color.g, node.color.b);
				TIndex trackId = peaks.GetIndexArray()[i];
				if(HasTrackId(trackId))
				{
					unsigned index=GetIndex(trackId);
					mTracks[index].push_back(node);
				}
				else
				{
					std::vector<SinTrackNode> newTrack;
					newTrack.push_back(node);
					mTracks.push_back(newTrack);
					AddToTable(trackId,mTracks.size()-1);
				}
			}
			mTime++;
		}
	
		void NetSinTracking::SetIndexes(const TIndex& left1, const TIndex& right1, 
										const TIndex& left2, const TIndex& right2, 
										const TIndex& begin)
		{
			mLeftIndex1=left1;
			mRightIndex1=right1;
			mLeftIndex2=left2;
			mRightIndex2=right2;
			mBegin=begin;
		}

    }
}

// END

