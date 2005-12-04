#include "CLAMGL.hxx"
#include "NetDataRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		NetDataRenderer::NetDataRenderer()
		{
		}
	
		NetDataRenderer::~NetDataRenderer()
		{
		}
	
		void NetDataRenderer::SetDataPtr(const TData* data,unsigned nElems)
		{
			mData = data;
			mElems = nElems;
		}
	
		void NetDataRenderer::Render()
		{
			glColor3ub(GLubyte(GetColor().r),GLubyte(GetColor().g),GLubyte(GetColor().b));
			glBegin(GL_LINE_STRIP);
			for(unsigned i = 0; i < mElems; i++)
			{
				glVertex3f(float(i),float(mData[i]),-1.0f);
			}
			glEnd();	
		}
	}
}

//END


