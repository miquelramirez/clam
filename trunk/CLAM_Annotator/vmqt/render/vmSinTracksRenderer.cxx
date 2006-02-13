#include "CLAMGL.hxx"
#include "vmSinTracksBuilder.hxx"
#include "vmSinTracksRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		SinTracksRenderer::SinTracksRenderer()
			: mglList(0)
			, mPeakMtxSize(0)
		{
		}

		SinTracksRenderer::~SinTracksRenderer()
		{
			if(mglList) glDeleteLists(mglList, mglList);
		}

		void SinTracksRenderer::SetData(const Array<SpectralPeakArray>& peakMtx)
		{
			mTracks.clear();
			mPeakMtxSize = peakMtx.Size();
			SinTracksBuilder tbuilder;
			mTracks = tbuilder.GetTracks(peakMtx);
			emit requestUpdate();
		}

		void SinTracksRenderer::Render()
		{
			if(!mEnabled) return;
			if(!mPeakMtxSize || !mTracks.size()) return;

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(mLocalView.left,mLocalView.right,mLocalView.bottom,mLocalView.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);

			if(!mglList)
			{
				mglList = GetglList();
				glNewList(mglList,GL_COMPILE_AND_EXECUTE);
				glLineWidth(1);
				for(unsigned i=0; i < mTracks.size(); i++)
				{
					glBegin(GL_LINE_STRIP);
					for(unsigned j=0; j < mTracks[i].size(); j++)
					{
						glColor3ub(mTracks[i][j].color.r, mTracks[i][j].color.g, mTracks[i][j].color.b);
						glVertex2d(double(mTracks[i][j].timeIndex),mTracks[i][j].freq);
					}
					glEnd();
				}
				glEndList();
			}
			else
			{
				glCallList(mglList);
			}
			
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void SinTracksRenderer::SetHBounds(double left, double right)
		{
			if(!mPeakMtxSize) return;
			mLocalView.left = (left-mXRange.min)*double(mPeakMtxSize)/mXRange.Span();
			mLocalView.right = (right-mXRange.min)*double(mPeakMtxSize)/mXRange.Span()-1.0;
		}

		void SinTracksRenderer::SetVBounds(double bottom, double top)
		{
			Renderer2D::SetVBounds(bottom,top);
			mLocalView.bottom = mView.bottom;
			mLocalView.top = mView.top-1.0;
		}
	}
}

// END
