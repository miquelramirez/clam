#include "CLAMGL.hxx"
#include "NetPeaksRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
	    	NetPeaksRenderer::NetPeaksRenderer()
		{
		    _nElems = 0;
		    SetPeakColor(VMColor::Cyan(),VMColor::Red());
		}

		NetPeaksRenderer::~NetPeaksRenderer()
		{
		}

		void NetPeaksRenderer::SetDataPtr(const TData* magBuff, const TData* freqBuff, int nElems)
		{
		    _magBuff = magBuff;
		    _freqBuff = freqBuff;
		    _nElems = nElems;
		}

		void NetPeaksRenderer::SetPeakColor(Color cline, Color cpoint)
		{
		    _cline = cline;
		    _cpoint = cpoint;
		}

		void NetPeaksRenderer::Render()
		{
		    glPointSize(2);

		    for(int i=0;i < _nElems;i++)
		    {
                        // draw point
			glColor3ub(GLubyte(_cpoint.r),GLubyte(_cpoint.g),GLubyte(_cpoint.b));
			glBegin(GL_POINTS);
			glVertex2f(float(_freqBuff[i]),float(_magBuff[i]));
			glEnd();

                        // draw vline
			glColor3ub(GLubyte(_cline.r),GLubyte(_cline.g),GLubyte(_cline.b));
			glBegin(GL_LINES);
			glVertex2f(float(_freqBuff[i]),float(_magBuff[i]));
			glVertex2f(float(_freqBuff[i]),float(_bottom));
			glEnd();
		    }
		}
	}	
}

// END


