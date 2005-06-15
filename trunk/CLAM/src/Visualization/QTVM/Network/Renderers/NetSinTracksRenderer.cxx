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
			mTracks = data;
		}

		void NetSinTracksRenderer::Render()
		{
			for(unsigned i=0; i < mTracks.size(); i++)
			{
				glBegin(GL_LINE_STRIP);
				for(unsigned j=0; j < mTracks[i].size(); j++)
				{
					glColor3ub(GLubyte(mTracks[i][j].color.r),GLubyte(mTracks[i][j].color.g),GLubyte(mTracks[i][j].color.b));
					glVertex2f(GLfloat(mTracks[i][j].timeIndex),GLfloat(mTracks[i][j].freq));
				}
				glEnd();
			}
		}

    }
}

// END

