#include "CLAMGL.hxx"
#include "BPFEditorRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	BPFEditorRenderer::BPFEditorRenderer()
	{
	    SetDataColor(VMColor::White());
	    SetHandlersColor(VMColor::White());
	}
	
	BPFEditorRenderer::~BPFEditorRenderer()
	{
	}

	void BPFEditorRenderer::SetData(const BPF& bpf)
	{
	    mData = bpf;
	}

	void BPFEditorRenderer::SetDataColor(const Color& c)
	{
	    mDataColor = c;
	}

	void BPFEditorRenderer::SetHandlersColor(const Color& c)
	{
	    mHandlersColor = c;
	}

	void BPFEditorRenderer::Render()
	{
	    DrawData();
	    DrawHandlers();
	}
	
	void BPFEditorRenderer::DrawData()
	{
	    glColor3ub(GLubyte(mDataColor.r),GLubyte(mDataColor.g),GLubyte(mDataColor.b));
	    glBegin(GL_LINE_STRIP);
	    for(TIndex i=0; i < mData.Size(); i++)
	    {
		glVertex2f(GLfloat(mData.GetXValue(i)),GLfloat(mData.GetValueFromIndex(i)));
	    }
	    glEnd();
	}

	void BPFEditorRenderer::DrawHandlers()
	{
	    glPointSize(POINT_SIZE);
	    glColor3ub(GLubyte(mHandlersColor.r),GLubyte(mHandlersColor.g),GLubyte(mHandlersColor.b));
	    glBegin(GL_POINTS);
	    for(TIndex i=0; i < mData.Size(); i++)
	    {
		glVertex2f(GLfloat(mData.GetXValue(i)),GLfloat(mData.GetValueFromIndex(i)));
	    }
	    glEnd();
	}
    }
}

// END
