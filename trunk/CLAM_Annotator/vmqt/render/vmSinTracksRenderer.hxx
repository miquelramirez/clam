#ifndef __VMQT_SINTRACKS_RENDERER_H__
#define __VMQT_SINTRACKS_RENDERER_H__

#include <vector>
#include "Array.hxx"
#include "SpectralPeakArray.hxx"
#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksRenderer : public Renderer2D
		{
		public:
			SinTracksRenderer();
			~SinTracksRenderer();

			void SetData(const Array<SpectralPeakArray>& peakMtx);

		protected:
			void Render();
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);

		private:
			int    mglList;
			TSize  mPeakMtxSize;
			GLView mLocalView;

			std::vector< std::vector<SinTrackNode> > mTracks;
		};
	}
}

#endif


