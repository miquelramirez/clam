#include <CLAM/CLAMGL.hxx>
#include <CLAM/BPFEditorRenderer.hxx>

namespace CLAM
{
    namespace VM
    {
		BPFEditorRenderer::BPFEditorRenderer()
			: mData(0)
			, mLeftIndex(0)
			, mRightIndex(0)
			, mSelectedIndex(-1)
			, mIsCurrentPainter(false)
		{
			SetDataColor(VMColor::White(),VMColor::White());
		}
	
		BPFEditorRenderer::~BPFEditorRenderer()
		{
		}

		void BPFEditorRenderer::SetData(const BPF& bpf)
		{
			mData = &bpf;
		}

		void BPFEditorRenderer::SetDataColor(const Color& lines_color, const Color& handlers_color)
		{
			mDataColor = lines_color;
			mHandlersColor = handlers_color;
		}

		void BPFEditorRenderer::Render()
		{
			if(!mData) return;
			if(!mData->Size()) return;
			if(!mReadPixels)
			{
				DrawPixels();
				return;
			}
//		  
			DrawData();
			DrawHandlers();

			if(mIsCurrentPainter) ReadPixels();
			
		}
					

		void BPFEditorRenderer::SetBounds(const TIndex& left, const TIndex& right)
		{
			mLeftIndex=left;
			mRightIndex=right;
		}
	
		void BPFEditorRenderer::DrawData()
		{
			glLineWidth(LINE_WIDTH);
			glColor3ub(GLubyte(mDataColor.r),GLubyte(mDataColor.g),GLubyte(mDataColor.b));
			glBegin(GL_LINE_STRIP);
			for(TIndex i=mLeftIndex; i <= mRightIndex; i++)
			{
				glVertex2f(GLfloat(mData->GetXValue(i)),GLfloat(mData->GetValueFromIndex(i)));
			}
			glEnd();
		}

		void BPFEditorRenderer::DrawHandlers()
		{
			glPointSize(POINT_SIZE);
			glColor3ub(GLubyte(mHandlersColor.r),GLubyte(mHandlersColor.g),GLubyte(mHandlersColor.b));
			glBegin(GL_POINTS);
			for(TIndex i=mLeftIndex; i <= mRightIndex; i++)
			{
				glVertex2f(GLfloat(mData->GetXValue(i)),GLfloat(mData->GetValueFromIndex(i)));
			}
			glEnd();

			if(mSelectedIndex < 0 || mSelectedIndex >= mData->Size()) return;
			glPointSize(POINT_SIZE+1);
			Color c;
			if(mHandlersColor==VMColor::Cyan())
			{
				c = VMColor::Orange();
			}
			else
			{
				c = VMColor::Red();
			}
			glColor3ub(GLubyte(c.r),GLubyte(c.g),GLubyte(c.b));
			glBegin(GL_POINTS);
			glVertex2f(GLfloat(mData->GetXValue(mSelectedIndex)),GLfloat(mData->GetValueFromIndex(mSelectedIndex)));
			glEnd();
			
		}

		void BPFEditorRenderer::SetSelectedIndex(const int& index)
		{
			mSelectedIndex = index;
		}

		void BPFEditorRenderer::CurrentPainter(bool cur)
		{
			mIsCurrentPainter = cur;
		}
    }
}

// END

