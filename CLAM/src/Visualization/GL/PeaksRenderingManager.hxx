#ifndef __PEAKSRENDERINGMANAGER__
#define __PEAKSRENDERINGMANAGER__

#include "GLRenderingManager.hxx"
#include "GLPointsArrays.hxx"
#include "DataTypes.hxx"
#include "Partial.hxx"
#include "Array.hxx"

namespace CLAMVM
{
	using CLAM::Array;
	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TSize;
	using CLAM::TIndex;

	class PeaksRenderingManager : public GLRenderingManager
	{
	public:
		PeaksRenderingManager();
		virtual ~PeaksRenderingManager();

		virtual void SetWorldBounds( const DataBoundBox& );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void SetSpectralRange( TData srange );
		virtual void SetBinNumber( TSize binNum );
		virtual void CacheData( const Array<Partial>& array );
		virtual void RenderData();

	protected:
				
		virtual void ProcessData();
		virtual void ConvertPartialsToArrays();
		virtual void DetermineVisibleSamples();

	private:
				
		Array<Partial>   mDataCached;
		DataArray        mProcessedX;
		DataArray        mProcessedY;
		GLPointsArrays    mPeakRenderer;
		DataBoundBox     mDataBBox;
		ScreenBoundBox   mScreenBBox;
		bool             mMustProcessData;
		bool             mNoPeaksToShow;
		TData            mSpecRange;
		TSize            mNumBins;
		TIndex           mLeftMostPeak;
		TIndex           mRightMostPeak;
	};

}


#endif // PeaksRenderingManager.hxx
