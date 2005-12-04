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
				glVertex3f(float(j++),float(mData[i]),-1.0f);
			}
			for(i=mLeftIndex2; i < mRightIndex2; i++)
			{
				glVertex3f(float(j++),float(mData[i]),-1.0f);
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

		void BuffDataRenderer::Init()
		{
			mLeftIndex1 = mRightIndex1 = mLeftIndex2 = mRightIndex2 = mBegin = 0;
		}
	}
}

// END

