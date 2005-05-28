#include "CLAMGL.hxx"
#include "SpectrogramRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	SpectrogramRenderer::SpectrogramRenderer()
	    : mLeftIndex(0),
	      mRightIndex(0),
	      mBottomIndex(0),
	      mTopIndex(0),
	      mColorMap(CLAM::VM::ColorSonogram),
	      mLineWidth(1.0f)
	{
	}

	SpectrogramRenderer::~SpectrogramRenderer()
	{
	}

	    
	void SpectrogramRenderer::SetIndexes(const TIndex& leftIndex, const TIndex& rightIndex,
					     const TIndex& bottomIndex, const TIndex& topIndex)
	{
	    mLeftIndex = leftIndex;
	    mRightIndex = rightIndex;
	    mBottomIndex = bottomIndex;
	    mTopIndex = topIndex;
	}

	void SpectrogramRenderer::SetColorData(std::vector< std::vector<Color> >& cdata)
	{
	    mColorData = cdata;
	}

	void SpectrogramRenderer::SetBWData(std::vector< std::vector<Color> >& bwdata)
	{
	    mBWData = bwdata;
	}

	void SpectrogramRenderer::Render()
	{
	    glLineWidth(GLfloat(mLineWidth));
	    switch(mColorMap)
	    {
		case CLAM::VM::ColorSonogram:
		    DrawColorData();
		    break;
		case CLAM::VM::BlackWhiteSonogram:
		    DrawBWData();
		default:
		    break;
	    }
	    glLineWidth(1.0);
	}

	void SpectrogramRenderer::SetRenderingMode(CLAM::VM::SonogramCM colorMap)
	{
	    mColorMap = colorMap;
	}

	void SpectrogramRenderer::SetLineWidth(const float& lineWidth)
	{
	    mLineWidth = lineWidth;
	}

	void SpectrogramRenderer::DrawColorData()
	{
	    for(TIndex i = mBottomIndex; i < mTopIndex; i++)
	    {
		glBegin(GL_LINE_STRIP);
		TIndex k=0;
		for(TIndex j = mLeftIndex; j < mRightIndex; j++)
		{
		    glColor3ub(GLubyte(mColorData[j][i].r),GLubyte(mColorData[j][i].g),GLubyte(mColorData[j][i].b));
		    glVertex2f(GLfloat(k++),GLfloat(i));
		}
		glEnd();
	    }
	}

	void SpectrogramRenderer::DrawBWData()
	{
	    for(TIndex i = mBottomIndex; i < mTopIndex; i++)
	    {
		glBegin(GL_LINE_STRIP);
		TIndex k=0;
		for(TIndex j = mLeftIndex; j < mRightIndex; j++)
		{
		    glColor3ub(GLubyte(mBWData[j][i].r),GLubyte(mBWData[j][i].g),GLubyte(mBWData[j][i].b));
		    glVertex2f(GLfloat(k++),GLfloat(i));
		}
		glEnd();
	    }
	}
    }
}

// END

