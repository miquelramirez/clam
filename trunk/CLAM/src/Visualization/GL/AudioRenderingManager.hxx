#ifndef __AUDIORENDERINGMANAGER__
#define __AUDIORENDERINGMANAGER__

#include "GLRenderingManager.hxx"
#include "GLMaxMinArrays.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAMVM
{

	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TSize;
	using CLAM::TTime;

	struct tAudioTimeInfo
	{
		TTime startTime;
		TTime endTime;
		TData sampleRate;
	};

	class GLStraightLineArray;
		

	class AudioRenderingManager : public GLRenderingManager
	{
	public:

		AudioRenderingManager();

		virtual void SetWorldBounds( const DataBoundBox&  );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void CacheData( const DataArray& array, const tAudioTimeInfo& nfo );
		virtual void RenderData();
		const tAudioTimeInfo& GetTimeInfo(  );
		const DataArray& GetDataCached(  );

		void SetDetailThreshold( int nSamples )
		{
			mDetailThreshold = nSamples;
		}

		virtual ~AudioRenderingManager();

	protected:

		virtual void ProcessData();
		
		void ActivateStraightRenderer( );
		
		void ActivateDetailRenderer( );
		
		void BuildMaxMinArrays( TSize offset, TSize len );
		
		void DetermineVisibleSamples();

	private:
		
		DataArray                mDataCached;
		DataArray                mProcessedData;
		DataArray                mMaxs;
		DataArray                mMins;
		GLStraightLineArray*     mActiveRenderer;
		GLMaxMinArrays           mHugeArrayRenderer;
		int                      mDetailThreshold;
		DataBoundBox             mDataBBox;
		ScreenBoundBox           mScreenBBox;
		bool                     mMustProcessData;
		bool                     mHugeArrayCondition;
		tAudioTimeInfo           mCachedTimeInfo;
		TSize                    mOffset;
	};

}
#endif // AudioRenderingManager.hxx
