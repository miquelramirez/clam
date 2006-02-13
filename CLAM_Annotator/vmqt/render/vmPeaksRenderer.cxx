#include "CLAMGL.hxx"
#include "vmPeaksRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		PeaksRenderer::PeaksRenderer()
			: mCpoint(255,0,0)
			, mCline(0,255,255)
		{
		}

		PeaksRenderer::~PeaksRenderer()
		{
		}

		void PeaksRenderer::SetPeaks(const Array<Peak>& peaks)
		{
			mCachedPeaks = peaks;
			requestUpdate();
		}

		void PeaksRenderer::SetPeaksColor(const Color& cpoint, const Color& cline)
		{
			mCpoint = cpoint;
			mCline = cline;
			requestRefresh();
		}

		void PeaksRenderer::Render()
		{
			if(!mEnabled || !mCachedPeaks.Size()) return;
			int nElems = mCachedPeaks.Size();
			// draw lines
			glLineWidth(LINE_WIDTH);
			glColor3ub(mCline.r,mCline.g,mCline.b);
			glBegin(GL_LINES);
			for(int i=0; i < nElems; i++)
			{
				if(mCachedPeaks[i].freq > mView.right) break;
				if(mCachedPeaks[i].freq >= mView.left)
				{
					glVertex2d(mCachedPeaks[i].freq,mView.bottom);
					glVertex2d(mCachedPeaks[i].freq,mCachedPeaks[i].mag);
				}
			}
			glEnd();
			// drqw points
			glPointSize(POINT_SIZE);
			glColor3ub(mCpoint.r,mCpoint.g,mCpoint.b);
			glBegin(GL_POINTS);
			for(int i=0; i < nElems; i++)
			{
				if(mCachedPeaks[i].freq > mView.right) break;
				if(mCachedPeaks[i].freq >= mView.left)
				{
					glVertex2d(mCachedPeaks[i].freq,mCachedPeaks[i].mag);
				}
			}
			glEnd();
			glLineWidth(1);
			glPointSize(1);
		}

		void PeaksRenderer::MouseMoveEvent(double x, double y)
		{
			if(!mEnabled) return;
			if(!mCachedPeaks.Size() || !mCatchEvents) return;
			int peak_index = PickPeak(x,y);
			if(peak_index != -1)
			{
				emit working(mKey,true);
				QString ttip = "freq="+QString::number(mCachedPeaks[peak_index].freq,'f',0)+"Hz "+"mag="+QString::number(mCachedPeaks[peak_index].mag,'f',0)+"dB";
				emit toolTip(ttip);
				return;
			}
			emit working(mKey,false);
			emit cursorChanged(QCursor(Qt::ArrowCursor));
			emit toolTip("");
		}
		
		int PeaksRenderer::PickPeak(double x, double y)
		{
			int index = -1;
			double xtolerance = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);
			double ytolerance = double(TOLERANCE)*(mView.top-mView.bottom)/double(mViewport.h);
			int left_index = 0;
			int right_index = mCachedPeaks.Size()-1;
			while(left_index <= right_index)
			{
				const int current_index = (left_index+right_index)/2;
			
				if(mCachedPeaks[current_index].freq >= (x-xtolerance) && 
				   mCachedPeaks[current_index].freq <= (x+xtolerance))
				{
					index = current_index;
					break;
				}
				if(x < mCachedPeaks[current_index].freq)
				{
					right_index = current_index-1;
				}
				else if(x > mCachedPeaks[current_index].freq)
				{
					left_index = current_index+1;
				}
			}
			if(index==-1) return -1;
			if(mCachedPeaks[index].mag >= (y-ytolerance)) return index;
			return -1;
		}
	}
}

// END
