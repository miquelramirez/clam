#include "CLAMGL.hxx"
#include "BuffDataRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	BuffDataRenderer::BuffDataRenderer()
	    : _nElems(0), _index(0)
	{
	    SetColor(VMColor::Green());
	}

	BuffDataRenderer::~BuffDataRenderer()
	{
	}

	void BuffDataRenderer::SetDataPtr(const TData* data, unsigned nElems, unsigned index)
	{
	    _data=data;
	    _nElems=nElems;
	    _index=index;
	}

	void BuffDataRenderer::Render()
	{
	    unsigned i,j=0;
	    glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
	    glBegin(GL_LINE_STRIP);
	    for(i=_index; i < _nElems; i++)
	    {
		glVertex2f((GLfloat)j++,(GLfloat)_data[i]);
	    }
	    for(i=0; i < _index; i++)
	    {
		glVertex2f((GLfloat)j++,(GLfloat)_data[i]);
	    }
	    glEnd();
	}
    }
}

// END

