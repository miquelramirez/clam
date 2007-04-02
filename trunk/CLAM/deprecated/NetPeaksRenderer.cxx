#include "CLAMGL.hxx"
#include "NetPeaksRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPeaksRenderer::NetPeaksRenderer()
			: mElems(0)
		{
		    SetPeakColor(VMColor::Cyan(),VMColor::Red());
		}

		NetPeaksRenderer::~NetPeaksRenderer()
		{
		}

		void NetPeaksRenderer::SetDataPtr(const TData* magBuff, const TData* freqBuff, int nElems)
		{
		    mMagBuff = magBuff;
		    mFreqBuff = freqBuff;
		    mElems = nElems;
		}

		void NetPeaksRenderer::SetPeakColor(const Color& cline, const Color& cpoint)
		{
		    mCline = cline;
		    mCpoint = cpoint;
		}

		void NetPeaksRenderer::Render()
		{
		    glPointSize(2);

		    for(int i=0;i < mElems;i++)
		    {
				// draw point
				glColor3ub(GLubyte(mCpoint.r),GLubyte(mCpoint.g),GLubyte(mCpoint.b));
				glBegin(GL_POINTS);
				glVertex2f(float(mFreqBuff[i]),float(mMagBuff[i]));
				glEnd();

				// draw vline
				glColor3ub(GLubyte(mCline.r),GLubyte(mCline.g),GLubyte(mCline.b));
				glBegin(GL_LINES);
				glVertex2f(float(mFreqBuff[i]),float(mMagBuff[i]));
				glVertex2f(float(mFreqBuff[i]),float(BottomBound()));
				glEnd();
		    }
		}
	}	
}

// END


