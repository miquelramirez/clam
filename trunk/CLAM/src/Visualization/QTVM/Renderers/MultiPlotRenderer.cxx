#include "CLAMGL.hxx"
#include "MultiPlotRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		MultiPlotRenderer::MultiPlotRenderer()
		{
			SetColor(VMColor::Blue());
		}

		MultiPlotRenderer::~MultiPlotRenderer()
		{
		}

		void MultiPlotRenderer::SetData(const DataArray& data)
		{
			mData = data;
		}

		void MultiPlotRenderer::Render()
		{
			if(!mData.Size()) return;
			glColor3ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b));
			glBegin(GL_LINE_STRIP);
			for(TIndex i = 0; i < mData.Size(); i++)
			{
				glVertex3f(float(i),float(mData[i]),-1.0f);
			}
			glEnd();			
		}
	}
}

// END

