#include "GLSineTracks.hxx"
#include "CLAMGL.hxx"
#include "Assert.hxx"
#include "Fl_Palette.hxx"
#include <iostream>

namespace CLAMVM
{
		GLSineTracks::GLSineTracks()
			: mHeads(NULL), mTails(NULL), mPalette( NULL )
		{
		}

		GLSineTracks::~GLSineTracks()
		{
				
		}

		void GLSineTracks::ExecuteGLCommands()
		{
				CLAM_ASSERT( mPalette!=NULL, "No palette was given!" );
				unsigned char colors[3];

				glClear( GL_COLOR_BUFFER_BIT );
				glMatrixMode( GL_MODELVIEW );
				glShadeModel( GL_SMOOTH );
				glLoadIdentity();

				SineTrackSpanEnds::iterator currStart, currEnd;
				
				currStart = mHeads->begin();
				currEnd = mTails->begin();

				while ( currStart != mHeads->end() && currEnd !=mTails->end() )
				{
						SinusoidalTrack::iterator trackBegin, trackEnd;
						trackBegin = *currStart;
						trackEnd = *currEnd;

						glBegin( GL_LINE_STRIP );
						while ( trackBegin != trackEnd )
						{
							if ( ((*trackBegin).mColorIndex < 64) || ((*trackBegin).mColorIndex >= 128 ) )
							{
								std::cerr << (*trackBegin).mMag << std::endl;
								std::cerr << (*trackBegin).mFreq << std::endl;
								std::cerr << (*trackBegin).mTimeIndex << std::endl;
								std::cerr << (*trackBegin).mColorIndex << std::endl;

								CLAM_ASSERT( false, "Out of range" );
							}


							mPalette->GetRGBFromIndex( (*trackBegin).mColorIndex, colors[0], colors[1], colors[2]);
							glColor3ubv( colors );
							glVertex2f( (*trackBegin).mTimeIndex, (*trackBegin).mFreq );
							trackBegin++;
						}
						glEnd();
						currStart++;
						currEnd++;
				}
				glShadeModel( GL_FLAT );
		}
}
