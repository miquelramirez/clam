#include "CLAMGL.hxx"
#include "NetSpecgramRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramRenderer::NetSpecgramRenderer()
	    : _index(0)
	{
	}

	NetSpecgramRenderer::~NetSpecgramRenderer()
	{
	}

	void NetSpecgramRenderer::SetData( std::vector< std::vector<Color> >& data, int index )
	{
	    if(!data.size()) return;
	    _data = data;
	    _index=index;
	}

	void NetSpecgramRenderer::Render()
	{
	    TSize dataSize = TSize(_data.size());
	    TSize specLen = (dataSize) ? _data[0].size() : 0;
	    for(int i = 0; i < specLen; i++)
	    {
		glBegin(GL_LINE_STRIP);
		int j,k=0;
		for(j = _index; j < dataSize; j++)
		{
		    if(i > (int)_data[j].size()-1) // ensure correct size
		    {
			break;
		    }
		    glColor3ub(GLubyte(_data[j][i].r),GLubyte(_data[j][i].g),GLubyte(_data[j][i].b));
		    glVertex2f(GLfloat(k++),GLfloat(i));
		}
		for(j = 0; j < _index; j++)
		{
		    if(i > (int)_data[j].size()-1) // ensure correct size
		    {
			break;
		    }
		    glColor3ub(GLubyte(_data[j][i].r),GLubyte(_data[j][i].g),GLubyte(_data[j][i].b));
		    glVertex2f(GLfloat(k++),GLfloat(i));
		}
		glEnd();
	    }
	}
    }
}

// END

