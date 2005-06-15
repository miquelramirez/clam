#include "CLAMGL.hxx"
#include "NetSpecgramRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
		NetSpecgramRenderer::NetSpecgramRenderer()
			: mBottomBound(0)
			, mTopBound(0)
			, mLeftIndex1(0)
			, mRightIndex1(0)
			, mLeftIndex2(0)
			, mRightIndex2(0)
			, mBegin(0)
		{
		}

		NetSpecgramRenderer::~NetSpecgramRenderer()
		{
		}

		void NetSpecgramRenderer::SetIndexes(const TIndex& leftIndex1, const TIndex& rightIndex1,
											 const TIndex& leftIndex2, const TIndex& rightIndex2,
											 const TIndex& begin)
		{
			mLeftIndex1 = leftIndex1;
			mRightIndex1 = rightIndex1;
			mLeftIndex2 = leftIndex2;
			mRightIndex2 = rightIndex2;
			mBegin = begin;
		}

		void NetSpecgramRenderer::SetData( std::vector< std::vector<Color> >& data, 
										   const TIndex&  bottom, 
										   const TIndex&  top )
		{
			if(!data.size()) return;
			mData = data;
			mBottomBound = bottom;
			mTopBound = top;
		}

		void NetSpecgramRenderer::Render()
		{
			for(TIndex i = mBottomBound; i < mTopBound; i++)
			{
				glBegin(GL_QUAD_STRIP);
				TIndex j,k=mBegin;
				glVertex2f(GLfloat(k),GLfloat(i));
				glVertex2f(GLfloat(k),GLfloat(i+1));
				for(j = mLeftIndex1; j < mRightIndex1; j++,k++)
				{
					if(i > (TIndex)mData[j].size()-1) // ensure correct size
					{
						break;
					}
					glColor3ub(GLubyte(mData[j][i].r),GLubyte(mData[j][i].g),GLubyte(mData[j][i].b));
					glVertex2f(GLfloat(k+1),GLfloat(i));
					glVertex2f(GLfloat(k+1),GLfloat(i+1));
				}
				for(j = mLeftIndex2; j < mRightIndex2; j++,k++)
				{
					if(i > (TIndex)mData[j].size()-1) // ensure correct size
					{
						break;
					}
					glColor3ub(GLubyte(mData[j][i].r),GLubyte(mData[j][i].g),GLubyte(mData[j][i].b));
					glVertex2f(GLfloat(k+1),GLfloat(i));
					glVertex2f(GLfloat(k+1),GLfloat(i+1));
				}
				glEnd();
			}
		}
    }
}

// END

