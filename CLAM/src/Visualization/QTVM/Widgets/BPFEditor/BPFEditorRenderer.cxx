#include "CLAMGL.hxx"
#include "BPFEditorRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	BPFEditorRenderer::BPFEditorRenderer()
	    : mSelectedIndex(-1)
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
	    glBegin(GL_POINTS);
	    for(TIndex i=0; i < mData.Size(); i++)
	    {
		if(i==mSelectedIndex)
		{
		    Color c;
		    if(mHandlersColor==VMColor::Cyan())
		    {
			c = VMColor::Orange();
		    }
		    else
		    {
			c = VMColor::Red();
		    }
		    glColor3ub(GLubyte(c.r),GLubyte(c.g),GLubyte(c.b));
		}
		else
		{
		    glColor3ub(GLubyte(mHandlersColor.r),GLubyte(mHandlersColor.g),GLubyte(mHandlersColor.b));
		}
		glVertex2f(GLfloat(mData.GetXValue(i)),GLfloat(mData.GetValueFromIndex(i)));
	    }
	    glEnd();
	}

	void BPFEditorRenderer::SetSelectedIndex(const int& index)
	{
	    mSelectedIndex = index;
	}
    }
}

// END
