#include "CLAMGL.hxx"
#include "BuffDataRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	BuffDataRenderer::BuffDataRenderer()
	    : _leftIndex1(0), 
	      _rightIndex1(0),
	      _leftIndex2(0),
	      _rightIndex2(0),
	      _begin(0)
	{
	    SetColor(VMColor::Green());
	}

	BuffDataRenderer::~BuffDataRenderer()
	{
	}

	void BuffDataRenderer::SetDataPtr(const TData* data)
	{
	    _data=data;
	}

	void BuffDataRenderer::Render()
	{
	    TIndex i,j=_begin;
	    glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
	    glBegin(GL_LINE_STRIP);
	    for(i=_leftIndex1; i < _rightIndex1; i++)
	    {
		glVertex2f((GLfloat)j++,(GLfloat)_data[i]);
	    }
	    for(i=_leftIndex2; i < _rightIndex2; i++)
	    {
		glVertex2f((GLfloat)j++,(GLfloat)_data[i]);
	    }
	    glEnd();
	}

	void BuffDataRenderer::SetIndexes(const TIndex& left1, const TIndex& right1, 
					  const TIndex& left2, const TIndex& right2,
					  const TIndex& begin)
	{
	    _leftIndex1=left1;
	    _rightIndex1=right1;
	    _leftIndex2=left2;
	    _rightIndex2=right2;
	    _begin=begin;
	}
    }
}

// END

