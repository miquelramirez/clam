#include "CLAMGL.hxx"
#include "vm_sintracks_builder.hxx"
#include "vm_sintracks_renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		SinTracksRenderer::SinTracksRenderer()
			: rd_glList(0)
			, rd_peakMtx_size(0)
		{
		}

		SinTracksRenderer::~SinTracksRenderer()
		{
			glDeleteLists(rd_glList, rd_glList);
		}

		void SinTracksRenderer::set_data(const Array<SpectralPeakArray>& peakMtx)
		{
			rd_tracks.clear();
			rd_peakMtx_size = peakMtx.Size();
			SinTracksBuilder tbuilder;
			rd_tracks = tbuilder.get_tracks(peakMtx);
			emit requestUpdate();
		}

		void SinTracksRenderer::render()
		{
			if(!rd_enabled) return;
			if(!rd_peakMtx_size || !rd_tracks.size()) return;

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(rd_local_view.left,rd_local_view.right,rd_local_view.bottom,rd_local_view.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);

			if(!rd_glList)
			{
				rd_glList = get_glList();
				glNewList(rd_glList,GL_COMPILE_AND_EXECUTE);
				glLineWidth(1);
				for(unsigned i=0; i < rd_tracks.size(); i++)
				{
					glBegin(GL_LINE_STRIP);
					for(unsigned j=0; j < rd_tracks[i].size(); j++)
					{
						glColor3ub(rd_tracks[i][j].color.r, rd_tracks[i][j].color.g, rd_tracks[i][j].color.b);
						glVertex2d(double(rd_tracks[i][j].timeIndex),rd_tracks[i][j].freq);
					}
					glEnd();
				}
				glEndList();
			}
			else
			{
				glCallList(rd_glList);
			}
			
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void SinTracksRenderer::set_hbounds(double left, double right)
		{
			if(!rd_peakMtx_size) return;
			rd_local_view.left = (left-rd_xrange.min)*double(rd_peakMtx_size)/rd_xrange.span();
			rd_local_view.right = (right-rd_xrange.min)*double(rd_peakMtx_size)/rd_xrange.span()-1.0;
		}

		void SinTracksRenderer::set_vbounds(double bottom, double top)
		{
			Renderer2D::set_vbounds(bottom,top);
			rd_local_view.bottom = rd_view.bottom;
			rd_local_view.top = rd_view.top-1.0;
		}
	}
}

// END
