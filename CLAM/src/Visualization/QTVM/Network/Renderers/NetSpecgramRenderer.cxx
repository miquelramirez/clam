#include <iostream>
#include "Assert.hxx"
#include "CLAMGL.hxx"
#include "NetSpecgramRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramRenderer::NetSpecgramRenderer()
	    : _palette(0.0f)	{
	}

	NetSpecgramRenderer::~NetSpecgramRenderer()
	{
	}

	void NetSpecgramRenderer::SetData( Array<DataArray>& data )
	{
	    if(!data.Size()) return;
	    _data = data;
	}

	void NetSpecgramRenderer::Render()
	{
	    TSize dataSize = TSize(_data.Size());
	    TSize specLen = _data[0].Size();
	    for(int i = 0; i < specLen; i++)
	    {
		glBegin(GL_LINE_STRIP);
		for(int j = 0;j < dataSize; j++)
		{
		    if(i > _data[j].Size()-1) // ensure correct size
		    {
			break;
		    }
		    TIndex colorIndex = _palette.Get( ClampToRange( _data[j][i]) );
		    if( colorIndex < 64 || colorIndex >= 128  )
		    {
			std::cerr << _data[j][i] << std::endl;
			std::cerr << colorIndex << std::endl;
			CLAM_ASSERT( false, "NetSpecgramRenderer: color index out of range" );
			}
		    _palette.GetRGBFromIndex( colorIndex, _color.r, _color.g, _color.b);
		    glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
		    glVertex2f(GLfloat(j),GLfloat(i));
		}
		glEnd();
	    }
	}

	float NetSpecgramRenderer::ClampToRange(TData value) const
	{
	    if ( value > 0.0 ) // 0 dB is the maximum
		return 1.0f;
	    if ( value < -100.0 ) // -100 dB is the minimum
		return 0.0f;

	    value += 100.0f;
	    value*= 0.01f;
	   
	    return value;
	}

    }
}

// END

