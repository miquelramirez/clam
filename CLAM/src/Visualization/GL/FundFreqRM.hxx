#ifndef __FUNDFREQRM__
#define __FUNDFREQRM__

#include "Array.hxx"
#include "TimeFreqMagPoint.hxx"
#include "Fl_Palette.hxx"
#include "GLRenderingManager.hxx"

namespace CLAMVM
{
	class FundFreqRM : public GLRenderingManager
	{
	public:
		FundFreqRM();
		virtual ~FundFreqRM();

		virtual void SetWorldBounds( const DataBoundBox& );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void CacheData( const CLAM::Array<TimeFreqMagPoint>& );
		virtual void RenderData();
		
	protected:

		virtual void ProcessData();
		virtual void DetermineVisibleSamples() {}
		virtual void Colorize();

	private:

		CLAM::Array<float>    mFreqBuffer;
		CLAM::Array<float>    mErrorBuffer;
		CLAM::Array<Fl_Color> mColorBuffer;
		Fl_Palette            mPalette;
		bool                  mMustProcessData;
		bool                  mMustColorize;
		DataBoundBox          mDataBBox;
		ScreenBoundBox        mScreenBBox;

	};
}

#endif // FundFreqRM.hxx
