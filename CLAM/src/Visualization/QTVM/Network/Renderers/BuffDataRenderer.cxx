#include "CLAMGL.hxx"
#include "BuffDataRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
		BuffDataRenderer::BuffDataRenderer()
			: mLeftIndex1(0)
			, mRightIndex1(0)
			, mLeftIndex2(0)
			, mRightIndex2(0)
			, mBegin(0)
		{
			SetColor(VMColor::Green());
		}

		BuffDataRenderer::~BuffDataRenderer()
		{
		}

		void BuffDataRenderer::SetDataPtr(const TData* data)
		{
			mData = data;
		}

		void BuffDataRenderer::Render()
		{
			TIndex i,j=mBegin;
			glColor3ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b));
			glBegin(GL_LINE_STRIP);
			for(i=mLeftIndex1; i < mRightIndex1; i++)
			{
				glVertex2f((GLfloat)j++,(GLfloat)mData[i]);
			}
			for(i=mLeftIndex2; i < mRightIndex2; i++)
			{
				glVertex2f((GLfloat)j++,(GLfloat)mData[i]);
			}
			glEnd();
		}

		void BuffDataRenderer::SetIndexes(const TIndex& left1, const TIndex& right1, 
										  const TIndex& left2, const TIndex& right2,
										  const TIndex& begin)
		{
			mLeftIndex1 = left1;
			mRightIndex1 = right1;
			mLeftIndex2 = left2;
			mRightIndex2 = right2;
			mBegin = begin;
		}
	}
}

// END

