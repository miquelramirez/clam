#include <vector>
#include "CLAMGL.hxx"
#include "SegmentEditor.hxx"

namespace CLAM
{
	namespace VM
	{
		SegmentRenderer::SegmentRenderer()
			: mSegmentation(0)
			, mColors(3)
			, mMargin(0.1)
		{
			Colorize();
		}

		SegmentRenderer::~SegmentRenderer()
		{
		}

		void SegmentRenderer::SetData(const Segmentation& s)
		{
			mSegmentation = &s;
		}

		void SegmentRenderer::Render()
		{
			unsigned nElems = mSegmentation->onsets().size();
			if(!nElems) return;
			unsigned current = mSegmentation->current();
			const Segmentation::TimePositions & beginnings = mSegmentation->onsets();
			const Segmentation::TimePositions & endings = mSegmentation->offsets();
			for(unsigned i=0;  i < nElems; i++)
			{
				if(beginnings[i] > RightBound()) break;
				bool isCurrent = (i==current);
				if(IsVisible(beginnings[i],endings[i],LeftBound(),RightBound())) 
				{
					DrawSegment(beginnings[i],endings[i],1.0,-1.0,isCurrent);
				}
					
			}
			if (current>=nElems) return; // no current
			if (IsVisible(beginnings[current],endings[current],LeftBound(),RightBound())) 
				StippledRect(beginnings[current],endings[current],1.0,-1.0);

		}
		
		void SegmentRenderer::DrawSegment(double left, double right, double top, double bottom, bool isCurrent)
		{
			int cindex = NORMAL; // TODO: check for selected segments
			int lineWidth = isCurrent ? CLINEWIDTH : NLINEWIDTH;
			glLineWidth(lineWidth);
			// fill rec
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glColor4ub(mColors[cindex].r,mColors[cindex].g,mColors[cindex].b,125);
			glBegin(GL_QUADS);
			PlainRect(left,right,top,bottom);
			glEnd();
			glDisable(GL_BLEND);
			// draw plain
			glColor3ub(mColors[cindex].r,mColors[cindex].g,mColors[cindex].b);
			glBegin(GL_LINE_STRIP);
			PlainRect(left,right,top,bottom);
			glEnd();
			glLineWidth(1);
		}

		void SegmentRenderer::PlainRect(double left, double right, double top, double bottom)
		{
			glVertex2f(float(left-LeftBound()),float(bottom+mMargin));
			glVertex2f(float(left-LeftBound()),float(top-mMargin));
			glVertex2f(float(right-LeftBound()),float(top-mMargin));
			glVertex2f(float(right-LeftBound()),float(bottom+mMargin));
			glVertex2f(float(left-LeftBound()),float(bottom+mMargin));
		}

		bool SegmentRenderer::IsVisible(double left, double right, double lBound, double rBound)
		{
			if(left >= lBound  && left <= rBound) return true;
			if(right >= lBound && right <= rBound) return true;
			if(left <= lBound && right >= rBound) return true;
			return false;
		}

		void SegmentRenderer::SetMargin(double margin)
		{
			mMargin = margin;
		}

		void SegmentRenderer::Colorize()
		{
			mColors[NORMAL] = Color(100,200,20);
			mColors[SELECTED] = Color(100,200,20); // TODO: add white
			mColors[STIPPLED] = Color(225,90,60);
		}

		void SegmentRenderer::StippledRect(double left, double right, double top, double bottom)
		{
			glLineWidth(1);
			// draw stippled rect
			glColor3ub(mColors[STIPPLED].r,mColors[STIPPLED].g,mColors[STIPPLED].b);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(FACTOR,PATTERN);
			glBegin(GL_LINE_STRIP);
			PlainRect(left,right,top,bottom);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			glLineWidth(1);
		}
	}
}

// END

