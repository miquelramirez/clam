#ifndef __SPECTRUMRENDERINGMANAGER__
#define __SPECTRUMRENDERINGMANAGER__

#include "GLRenderingManager.hxx"
#include "GLStraightLineArray.hxx"
#include "GLSampleDetailArray.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAMVM
{
	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TSize;

	class SpectrumRenderingManager : public GLRenderingManager
	{
	public:
		SpectrumRenderingManager();
		virtual ~SpectrumRenderingManager();

		virtual void SetWorldBounds( const DataBoundBox&  );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void CacheData( const DataArray& array );
		virtual void RenderData();
				
		void SetDetailThreshold( int nSamples )
		{
			mDetailThreshold = nSamples;
		}
				
	protected:
		
		virtual void ProcessData();
				
		void ActivateStraightRenderer( );
				
		void ActivateDetailRenderer( );

		void DetermineVisibleSamples();

	private:			
				
		DataArray                mDataCached;
		DataArray                mProcessedData;
		GLStraightLineArray      mLineRenderer;
		bool                     mDrawAsLines;
		GLSampleDetailArray      mDetailRenderer;
		bool                     mDrawAsDetail;
		int                      mDetailThreshold;
		bool                     mMustProcessData;
		DataBoundBox             mDataBBox;
		ScreenBoundBox           mScreenBBox;
		TSize                    mOffset;
		TSize                    mLen;
	};
		
}

#endif // SpectrumRenderingManager.hxx
