#include <iostream>
#include "Assert.hxx"
#include "CLAMGL.hxx"
#include "NetSpecgramRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSpecgramRenderer::NetSpecgramRenderer()
	    : _palette(0.0f)
	{
	}

	NetSpecgramRenderer::~NetSpecgramRenderer()
	{
	}

	void NetSpecgramRenderer::SetData( std::vector<Spectrum>& data )
	{
	    _data = data;
	}

	void NetSpecgramRenderer::Render()
	{
	    glBegin(GL_POINTS);
	    for(int i = 0; (unsigned)i < _data.size(); i++)
	    {
		for(int j = 0;j < _data[i].GetMagBuffer().Size(); j++)
		{
		    TIndex colorIndex = _palette.Get( ClampToRange( _data[i].GetMagBuffer()[j]) );
		    if( colorIndex < 64 || colorIndex >= 128  )
		    {
			std::cerr << _data[i].GetMagBuffer()[j] << std::endl;
			std::cerr << i << std::endl;
			std::cerr << colorIndex << std::endl;
			CLAM_ASSERT( false, "NetSpecgramRenderer: color index out of range" );
		    }
		    _palette.GetRGBFromIndex( colorIndex, _color.r, _color.g, _color.b);
		    glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
		    glVertex2f(GLfloat(i),GLfloat(j));
		}
	    }
	    glEnd();
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

