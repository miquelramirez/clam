#include "CLAMGL.hxx"
#include "NetSpecgramRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramRenderer::NetSpecgramRenderer()
	{
	}

	NetSpecgramRenderer::~NetSpecgramRenderer()
	{
	}

	void NetSpecgramRenderer::SetData( std::vector< std::vector<Color> >& data )
	{
	    if(!data.size()) return;
	    _data = data;
	}

	void NetSpecgramRenderer::Render()
	{
	    TSize dataSize = TSize(_data.size());
	    TSize specLen = _data[0].size();
	    for(int i = 0; i < specLen; i++)
	    {
		glBegin(GL_LINE_STRIP);
		for(int j = 0;j < dataSize; j++)
		{
		    if(i > (int)_data[j].size()-1) // ensure correct size
		    {
			break;
		    }
		    glColor3ub(GLubyte(_data[j][i].r),GLubyte(_data[j][i].g),GLubyte(_data[j][i].b));
		    glVertex2f(GLfloat(j),GLfloat(i));
		}
		glEnd();
	    }
	}
    }
}

// END

