#include "CLAMGL.hxx"
#include "NetSinTracksRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSinTracksRenderer::NetSinTracksRenderer()
	{
	}

	NetSinTracksRenderer::~NetSinTracksRenderer()
	{
	}

	void NetSinTracksRenderer::SetData( std::vector< std::vector<SinTrackNode> >& data )
	{
	    _tracks = data;
	}

	void NetSinTracksRenderer::Render()
	{
	    for(unsigned i=0; i < _tracks.size(); i++)
	    {
		glBegin(GL_LINE_STRIP);
		for(unsigned j=0; j < _tracks[i].size(); j++)
		{
		    glColor3ub(GLubyte(_tracks[i][j].color.r),GLubyte(_tracks[i][j].color.g),GLubyte(_tracks[i][j].color.b));
		    glVertex2f(GLfloat(_tracks[i][j].timeIndex),GLfloat(_tracks[i][j].freq));
		}
		glEnd();
	    }
	}

    }
}

// END

