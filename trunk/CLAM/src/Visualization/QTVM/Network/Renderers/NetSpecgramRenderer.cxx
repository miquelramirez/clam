#include "CLAMGL.hxx"
#include "NetSpecgramRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramRenderer::NetSpecgramRenderer()
	    : _bottomBound(0),
	      _topBound(0),
	      _leftIndex1(0),
	      _rightIndex1(0),
	      _leftIndex2(0),
	      _rightIndex2(0),
	      _begin(0)
	{
	}

	NetSpecgramRenderer::~NetSpecgramRenderer()
	{
	}

	void NetSpecgramRenderer::SetIndexes(const TIndex& leftIndex1, const TIndex& rightIndex1,
					     const TIndex& leftIndex2, const TIndex& rightIndex2,
					     const TIndex& begin)
	{
	    _leftIndex1=leftIndex1;
	    _rightIndex1=rightIndex1;
	    _leftIndex2=leftIndex2;
	    _rightIndex2=rightIndex2;
	    _begin=begin;
	}

	void NetSpecgramRenderer::SetData( std::vector< std::vector<Color> >& data, 
					   const TIndex&  bottom, const TIndex&  top )
	{
	    if(!data.size()) return;
	    _data = data;
	    _bottomBound=bottom;
	    _topBound=top;
	}

	void NetSpecgramRenderer::Render()
	{
	    for(TIndex i = _bottomBound; i < _topBound; i++)
	    {
		glBegin(GL_QUAD_STRIP);
		TIndex j,k=_begin;
		glVertex2f(GLfloat(k),GLfloat(i));
		glVertex2f(GLfloat(k),GLfloat(i+1));
		for(j = _leftIndex1; j < _rightIndex1; j++,k++)
		{
		    if(i > (TIndex)_data[j].size()-1) // ensure correct size
		    {
			break;
		    }
		    glColor3ub(GLubyte(_data[j][i].r),GLubyte(_data[j][i].g),GLubyte(_data[j][i].b));
		    glVertex2f(GLfloat(k+1),GLfloat(i));
		    glVertex2f(GLfloat(k+1),GLfloat(i+1));
		}
		glVertex2f(GLfloat(k),GLfloat(i));
		glVertex2f(GLfloat(k),GLfloat(i+1));
		for(j = _leftIndex2; j < _rightIndex2; j++,k++)
		{
		    if(i > (TIndex)_data[j].size()-1) // ensure correct size
		    {
			break;
		    }
		    glColor3ub(GLubyte(_data[j][i].r),GLubyte(_data[j][i].g),GLubyte(_data[j][i].b));
		    glVertex2f(GLfloat(k+1),GLfloat(i));
		    glVertex2f(GLfloat(k+1),GLfloat(i+1));
		}
		glEnd();
	    }
	}
    }
}

// END

