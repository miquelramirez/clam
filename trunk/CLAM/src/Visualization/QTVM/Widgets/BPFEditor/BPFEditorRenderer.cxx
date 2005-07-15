#include "CLAMGL.hxx"
#include "BPFEditorRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
		BPFEditorRenderer::BPFEditorRenderer()
			: mLeftIndex(0)
			, mRightIndex(0)
			, mSelectedIndex(-1)
		{
			SetDataColor(VMColor::White(),VMColor::White());
		}
	
		BPFEditorRenderer::~BPFEditorRenderer()
		{
		}

		void BPFEditorRenderer::SetData(const BPF& bpf)
		{
			mData = bpf;
		}

		void BPFEditorRenderer::SetDataColor(const Color& lines_color, const Color& handlers_color)
		{
			mDataColor = lines_color;
			mHandlersColor = handlers_color;
		}

		void BPFEditorRenderer::Render()
		{
			DrawData();
			DrawHandlers();
		}

		void BPFEditorRenderer::SetBounds(const TIndex& left, const TIndex& right)
		{
			mLeftIndex=left;
			mRightIndex=right;
		}
	
		void BPFEditorRenderer::DrawData()
		{
			glColor3ub(GLubyte(mDataColor.r),GLubyte(mDataColor.g),GLubyte(mDataColor.b));
			glBegin(GL_LINE_STRIP);
			for(TIndex i=mLeftIndex; i < mRightIndex; i++)
			{
				glVertex2f(GLfloat(mData.GetXValue(i)),GLfloat(mData.GetValueFromIndex(i)));
			}
			glEnd();
		}

		void BPFEditorRenderer::DrawHandlers()
		{
			if(mSelectedIndex >=0 && mSelectedIndex < mData.Size())
			{
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
				glVertex2f(GLfloat(mData.GetXValue(mSelectedIndex)),GLfloat(mData.GetValueFromIndex(mSelectedIndex)));
				glEnd();
			}

			glPointSize(POINT_SIZE);
			glColor3ub(GLubyte(mHandlersColor.r),GLubyte(mHandlersColor.g),GLubyte(mHandlersColor.b));
			glBegin(GL_POINTS);
			for(TIndex i=mLeftIndex; i < mRightIndex; i++)
			{
				if(i != mSelectedIndex)
				{
					glVertex2f(GLfloat(mData.GetXValue(i)),GLfloat(mData.GetValueFromIndex(i)));
				}
		
			}
			glEnd();
		}

		void BPFEditorRenderer::SetSelectedIndex(const int& index)
		{
			mSelectedIndex = index;
		}
    }
}

// END
