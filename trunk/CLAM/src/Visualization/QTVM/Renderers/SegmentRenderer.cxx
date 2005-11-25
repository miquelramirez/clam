#include <vector>
#include "CLAMGL.hxx"
#include "SegmentEditor.hxx"

namespace CLAM
{
	namespace VM
	{
		SegmentRenderer::SegmentRenderer()
			: mSegmentation(0)
			, mMargin(0.1)
		{
			// Alt: 225,90,60 
			SetColor(VMColor::Custom(100,200,20));
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
			int type = NORMAL;
			const Segmentation::TimePositions & beginnings = mSegmentation->onsets();
			const Segmentation::TimePositions & endings = mSegmentation->offsets();
			for(unsigned i=0;  i < nElems; i++)
			{
				if(beginnings[i] > RightBound()) break;
				if(i >= 0)
				{
					if(current == i && nElems > 1) type = CURRENT;
				}
				if(IsVisible(beginnings[i],endings[i],LeftBound(),RightBound())) 
				{
					DrawSegment(beginnings[i],endings[i],0.9,type);
				}
				type = NORMAL;
			}
		}
		
		void SegmentRenderer::DrawSegment(double left, double right, double top, int type)
		{
			// TODO: check for highlighted: selected type
			int lineWidth = (type == CURRENT) ? CLINEWIDTH : NLINEWIDTH;
			glLineWidth(lineWidth);
			// draw border
			glColor3ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b));
			glBegin(GL_LINE_STRIP);
			Rect(left,right,top);
			glEnd();
			glLineWidth(1);
			// fill rec
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glColor4ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b),150);
			// draw cuad
			glBegin(GL_QUADS);
			Rect(left,right,top);
			glEnd();
			glDisable(GL_BLEND);
		}

		void SegmentRenderer::Rect(double left, double right, double top)
		{
			glVertex2f(float(left-LeftBound()),float(BottomBound()));
			glVertex2f(float(left-LeftBound()),float(top));
			glVertex2f(float(right-LeftBound()),float(top));
			glVertex2f(float(right-LeftBound()),float(BottomBound()));
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
	}
}

// END

