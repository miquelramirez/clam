#include "CLAMGL.hxx"
#include "VLineArrayRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		VLineArrayRenderer::VLineArrayRenderer()
		{
			SetColor(VMColor::Red());
		}

		VLineArrayRenderer::~VLineArrayRenderer()
		{
		}

		void VLineArrayRenderer::SetData(const Array<unsigned>& data)
		{
			_data = data;
		}

		void VLineArrayRenderer::Render()
		{
			for(int i=0;i < _data.Size();i++)
			{
				// draw vline
				glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
				glBegin(GL_LINES);
					glVertex2f(float(_data[i]-_bounds.left),float(_top));
					glVertex2f(float(_data[i]-_bounds.left),float(_bottom));
				glEnd();
			}
		}
	}
}

// END

