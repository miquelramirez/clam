#ifndef __SINTRACKSRENDERINGMANAGER__
#define __SINTRACKSRENDERINGMANAGER__

#include "GLRenderingManager.hxx"
#include "SineTracksDef.hxx"
#include "SinTrackHorClipper.hxx"
#include "SinTrackVerClipper.hxx"
#include "GLSineTracks.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Fl_Palette.hxx"

namespace CLAMVM
{
		using CLAM::Array;
		using CLAM::TSize;
		using CLAM::TIndex;
		using CLAM::TData;
		
		class SinTracksRM : public GLRenderingManager
		{
		public:
				SinTracksRM();
				virtual ~SinTracksRM();

				virtual void SetWorldBounds( const DataBoundBox& );
				virtual void SetPixelBounds( const ScreenBoundBox& );
				virtual void CacheData( SineTrackList& );
				virtual void RenderData();

		protected:
				virtual void ProcessData();
				virtual void DetermineVisibleSamples();
				virtual void Colorize();

				inline float ClampToRange( TData value );
				
		private:
				SineTrackList*       mCachedTracks;
				SinTrackHorClipper   mHorClipper;
				SinTrackVerClipper   mVerClipper;
				GLSineTracks         mRenderer;
				bool                 mMustProcessData;
				DataBoundBox         mDataBBox;
				ScreenBoundBox       mScreenBBox;
				SineTrackSpanEnds    mStarts;
				SineTrackSpanEnds    mEnds;
				Fl_Palette           mPalette;
				bool                 mColorsAssigned;
		};
}

#endif // SinTracksRenderingManager.hxx
