#include "CLAMGL.hxx"
#include "SpectralPeaksRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectralPeaksRenderer::SpectralPeaksRenderer()
		{
			SetPeakColor(VMColor::Cyan(),VMColor::Red());
		}

		SpectralPeaksRenderer::~SpectralPeaksRenderer()
		{
		}

		void SpectralPeaksRenderer::SetDataPtr(const TData* data,int nElems)
		{
			_data = data;
			_nElems = nElems;
		}

		void SpectralPeaksRenderer::SetPeakColor(Color cline,Color cpoint)
		{
			_cLine = cline;
			_cPoint = cpoint;
		}

		void SpectralPeaksRenderer::Render()
		{
			// points: (i,data[i])
			// vlines: top    (i,data[i]) 
			//		   bottom (i,_bottom)
			glPointSize(2);
			for(int i=0;i < _nElems;i++)
			{
				if(_data[i] < 150)
				{
					// draw point
					glColor3ub(GLubyte(_cPoint.r),GLubyte(_cPoint.g),GLubyte(_cPoint.b));
					glBegin(GL_POINTS);
						glVertex2f(float(i),float(_data[i]));
					glEnd();

					// draw vline
					glColor3ub(GLubyte(_cLine.r),GLubyte(_cLine.g),GLubyte(_cLine.b));
					glBegin(GL_LINES);
						glVertex2f(float(i),float(_data[i]));
						glVertex2f(float(i),float(_bottom));
					glEnd();
				}
			}
		}
	}
}

// END

