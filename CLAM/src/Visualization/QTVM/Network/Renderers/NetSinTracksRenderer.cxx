#include <iostream>
#include "Assert.hxx"
#include "CLAMGL.hxx"
#include "NetSinTracksRenderer.hxx"

namespace CLAM
{
    namespace VM
    {
	NetSinTracksRenderer::NetSinTracksRenderer()
	    : _palette(0.0f)
	{
	}

	NetSinTracksRenderer::~NetSinTracksRenderer()
	{
	}

	void NetSinTracksRenderer::SetData( SineTrackList& data )
	{
	    _trackList = data;
	}

	void NetSinTracksRenderer::Render()
	{
	    SineTrackList::iterator i = _trackList.begin();
	    SineTrackList::iterator end = _trackList.end();

	    while ( i != end )
	    {
		SinusoidalTrack::iterator itrack = i->begin();
		SinusoidalTrack::iterator endtrack = i->end();
		
		glBegin(GL_LINE_STRIP);
		while( itrack!=endtrack )
		{
		    if(itrack->mTimeIndex >= _bounds.left && itrack->mTimeIndex <= _bounds.right)
		    {
			itrack->mColorIndex = _palette.Get( ClampToRange( itrack->mMag ) );
			if( itrack->mColorIndex < 64 || itrack->mColorIndex >= 128  )
			{
			    std::cerr << itrack->mMag << std::endl;
			    std::cerr << itrack->mFreq << std::endl;
			    std::cerr << itrack->mTimeIndex << std::endl;
			    std::cerr << itrack->mColorIndex << std::endl;
			    CLAM_ASSERT( false, "Out of range" );
			}
		    
			_palette.GetRGBFromIndex( itrack->mColorIndex, _color.r, _color.g, _color.b);
			glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
			glVertex2f(GLfloat(itrack->mTimeIndex),GLfloat(itrack->mFreq));
		    }
		    itrack++;
		}
		glEnd();
		i++;
	    }

	}

	float NetSinTracksRenderer::ClampToRange(TData value) const
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

