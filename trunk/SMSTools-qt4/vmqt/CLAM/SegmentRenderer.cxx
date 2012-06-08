#include <vector>
#include <CLAM/CLAMGL.hxx>
#include <CLAM/SegmentEditor.hxx>

namespace CLAM
{
	namespace VM
	{
		SegmentRenderer::SegmentRenderer()
			: mSegmentation(0)
			, mColors(5)
			, mMargin(0.1)
			, mHighlighted(-1)
			, mBoundType(LEFT_BOUND)
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
//				bool isCurrent = (i==current);
				if(IsVisible(beginnings[i],endings[i],LeftBound(),RightBound())) 
				{
					DrawSegment(beginnings[i],endings[i],1.0,-1.0,i==current,mSegmentation->selections()[i]);
				}				
			}
			DrawVHighlighted(1.0,-1.0);
			if (current>=nElems) return; // no current
			if (IsVisible(beginnings[current],endings[current],LeftBound(),RightBound())) 
				StippledRect(beginnings[current],endings[current],1.0,-1.0);


		}
		
		void SegmentRenderer::DrawSegment(double left, double right, double top, double bottom,
										  bool isCurrent, bool isSelected)
		{
			int cindex = isSelected ? SELECTED : NORMAL;
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
			mColors[CURRENT] = Color(100,200,20);
			mColors[SELECTED] = Color(140,240,60); 
			mColors[HIGHLIGHTED] = Color(255,255,0);
			mColors[STIPPLED] = Color(255,90,60);
		}

		void SegmentRenderer::StippledRect(double left, double right, double top, double bottom)
		{
/*
			// TODO: Paddings computations works for me but that's a kludge!
			float vPadding = mMargin * 0.8;
			float hPadding = 4 * (RightBound()-LeftBound()) / 1000;
*/
			// IMR: new paddings (v=2 pixels, h=3 pixels)
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			float vPadding = mMargin-(2.0*(TopBound()-BottomBound())/float(viewport[3]));
			float hPadding = 3.0*(RightBound()-LeftBound())/float(viewport[2]);

			glLineWidth(2);
			// draw stippled rect
			glColor3ub(mColors[STIPPLED].r,mColors[STIPPLED].g,mColors[STIPPLED].b);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(FACTOR,PATTERN);
			glBegin(GL_LINE_STRIP);
			PlainRect(left-hPadding,right+hPadding,top+vPadding,bottom-vPadding);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			glLineWidth(1);
		}

		void SegmentRenderer::SetVHighlighted(int index, BoundType type)
		{
			mHighlighted = index;
			mBoundType = type;
		}

		void SegmentRenderer::DrawVHighlighted(double top, double bottom)
		{
			if(mHighlighted < 0 || mHighlighted >= (int)mSegmentation->onsets().size()) return;
			float x = 
				(mBoundType==LEFT_BOUND) 
				? float(mSegmentation->onsets()[mHighlighted]-LeftBound()) 
				: float(mSegmentation->offsets()[mHighlighted]-LeftBound());
			bool isCurrent = (mHighlighted == (int)mSegmentation->current());
			int lineWidth = isCurrent ? CLINEWIDTH : NLINEWIDTH;
			glLineWidth(lineWidth);
			// draw vertical highlighted 
			glColor3ub(mColors[HIGHLIGHTED].r,mColors[HIGHLIGHTED].g,mColors[HIGHLIGHTED].b);
			glBegin(GL_LINES);
			glVertex2f(x,float(top-mMargin)); 
			glVertex2f(x,float(bottom+mMargin));
			glEnd();
			glLineWidth(1);
		}
	}
}

// END

