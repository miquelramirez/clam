#include "CLAMGL.hxx"
#include "vm_sonogram_builder.hxx"
#include "vm_spectrogram_renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectrogramRenderer::SpectrogramRenderer()
			: rd_glColorList(0)
			, rd_glBlackWhiteList(0)
			, rd_colormap(COLOR_SONOGRAM)
		{
		}

		SpectrogramRenderer::~SpectrogramRenderer()
		{
		}

		void SpectrogramRenderer::set_data(const CLAM::Array<CLAM::Spectrum>& specMtx)
		{
			if(!specMtx.Size()) return;
			rd_computed_data.clear();
			rd_color_data.clear();
			rd_blackwhite_data.clear();
			CLAM::VM::SonogramBuilder sbuilder;
			sbuilder.make_sonogram(specMtx,rd_computed_data,rd_color_data,rd_blackwhite_data);
			emit requestUpdate();
			emit fixedLabels(QString::number(specMtx[0].GetSpectralRange(),'f',0),
							 QString::number(rd_computed_data.size()));
		}

		void SpectrogramRenderer::colorSonogram()
		{
			rd_colormap = COLOR_SONOGRAM;
			emit requestRefresh();
		}

		void SpectrogramRenderer::blackWhiteSonogram()
		{
			rd_colormap = BW_SONOGRAM;
			emit requestRefresh();
		}

		void SpectrogramRenderer::render()
		{
			if(!rd_enabled) return;
			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(rd_local_view.left,rd_local_view.right,rd_local_view.bottom,rd_local_view.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);

			switch(rd_colormap)
			{
				case COLOR_SONOGRAM:
					draw_color_data();
					break;
				case BW_SONOGRAM:
					draw_blackwhite_data();
					break;
				default:
					break;
			}

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);

		}

		void SpectrogramRenderer::set_hbounds(double left, double right)
		{
			if(!rd_color_data.size()) return;
			rd_local_view.left = (left-rd_xrange.min)*double(rd_color_data.size())/rd_xrange.span();
			rd_local_view.right = (right-rd_xrange.min)*double(rd_color_data.size())/rd_xrange.span()-1.0;
		}

		void SpectrogramRenderer::set_vbounds(double bottom, double top)
		{
			if(!rd_color_data.size()) return;
			if(!rd_color_data[0].size()) return;
			rd_local_view.bottom = (bottom-rd_yrange.min)*double(rd_color_data[0].size())/rd_yrange.span();
			rd_local_view.top = (top-rd_yrange.min)*double(rd_color_data[0].size())/rd_yrange.span()-1.0;
 		}

		void SpectrogramRenderer::mouse_move_event(double x, double y)
		{
			if(!rd_color_data.size()) return;
			if(!rd_color_data[0].size()) return;
			if(x < rd_xrange.min || x > rd_xrange.max) return;
			if(y < rd_yrange.min || y > rd_yrange.max) return;
			
			int i = int((x-rd_xrange.min)*double(rd_color_data.size())/rd_xrange.span());
			int j = int((y-rd_yrange.min)*double(rd_color_data[0].size())/rd_yrange.span());
			
			if(i < 0 || i >= (int)rd_color_data.size()) return;
			if(j < 0 || j >= (int)rd_color_data[0].size()) return;
			
			emit labels(QString::number(y,'f',0),
						QString::number(rd_computed_data[i][j],'f',0),
						QString::number(i),
						QString::number(x,'f',2));
		}

		void SpectrogramRenderer::leave_event()
		{
			emit labels("--","--","--","--");
		}

		void SpectrogramRenderer::draw_color_data()
		{
			if(!rd_color_data.size()) return;
			if(!rd_color_data[0].size()) return;
			if(!rd_glColorList)
			{
				rd_glColorList = get_glList();
				glNewList(rd_glColorList,GL_COMPILE_AND_EXECUTE);
				for(unsigned i = 0; i < rd_color_data[i].size(); i++)
				{
					glBegin(GL_QUAD_STRIP);
					unsigned k = 0;
					glVertex2d(GLdouble(k),GLdouble(i));
					glVertex2d(GLdouble(k),GLdouble(i+1));
					for(unsigned j = 0; j < rd_color_data.size(); j++,k++)
					{
						glColor3ub(rd_color_data[j][i].r,rd_color_data[j][i].g,rd_color_data[j][i].b);
						glVertex2d(GLdouble(k+1),GLdouble(i));
						glVertex2d(GLdouble(k+1),GLdouble(i+1));
					}
					glEnd();
				}
				glEndList();
			}
			else
			{
				glCallList(rd_glColorList);
			}
		}

		void SpectrogramRenderer::draw_blackwhite_data()
		{
			if(!rd_blackwhite_data.size()) return;
			if(!rd_blackwhite_data[0].size()) return;
			if(!rd_glBlackWhiteList)
			{
				rd_glBlackWhiteList = get_glList();
				glNewList(rd_glBlackWhiteList,GL_COMPILE_AND_EXECUTE);
				for(unsigned i = 0; i < rd_blackwhite_data[i].size(); i++)
				{
					glBegin(GL_QUAD_STRIP);
					unsigned k = 0;
					glVertex2d(GLdouble(k),GLdouble(i));
					glVertex2d(GLdouble(k),GLdouble(i+1));
					for(unsigned j = 0; j < rd_blackwhite_data.size(); j++,k++)
					{
						glColor3ub(rd_blackwhite_data[j][i].r,rd_blackwhite_data[j][i].g,rd_blackwhite_data[j][i].b);
						glVertex2d(GLdouble(k+1),GLdouble(i));
						glVertex2d(GLdouble(k+1),GLdouble(i+1));
					}
					glEnd();
				}
				glEndList();
			}
			else
			{
				glCallList(rd_glBlackWhiteList);
			}
		}

		int SpectrogramRenderer::get_glList()
		{
			int gl_list = 1;
			while(glIsList(gl_list)) gl_list++;
			return gl_list;
		}
	}
}

// END
