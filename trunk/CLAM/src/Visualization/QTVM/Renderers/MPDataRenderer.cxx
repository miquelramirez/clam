#include "CLAMGL.hxx"
#include "MPDataRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		MPDataRenderer::MPDataRenderer()
		{
			SetColor(VMColor::Blue());
		}

		MPDataRenderer::~MPDataRenderer()
		{
		}

		void MPDataRenderer::SetData(const TData* data,unsigned nElems)
		{
			_data.Resize(nElems);
			_data.SetSize(nElems);
			for(int i=0; i < _data.Size(); i++)
			{
				_data[i]=data[i];
			}
		}

		void MPDataRenderer::Render()
		{
			glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
			glBegin(GL_LINE_STRIP);
			for(unsigned i = 0; i < unsigned(_data.Size()); i++)
			{
				glVertex3f(float(i),float(_data[i]),-1.0f);
			}
			glEnd();
		}
	}
}

// END

