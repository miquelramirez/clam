#include "GLStraightLineArray.hxx"
#include "CLAMGL.hxx"
#include <cstdlib>

namespace CLAMVM
{
	GLStraightLineArray::GLStraightLineArray()
				: mElemsToDraw( 0 ), mValues( NULL ), mXStart( 0 ), mXHopSize( 1 )
		{
				mLineColor[0] = 0;
				mLineColor[1] = 255;
				mLineColor[2] = 0;
		}


		GLStraightLineArray::~GLStraightLineArray()
		{
		}

		void GLStraightLineArray::ExecuteGLCommands()
		{
				glClear( GL_COLOR_BUFFER_BIT );
				glMatrixMode( GL_MODELVIEW );
				glLoadIdentity();

				TData xvalue = mXStart;
				const TData* curryvalue = mValues;

				glColor3ubv( mLineColor );
				glBegin( GL_LINES );

				for ( unsigned i = 0; i < mElemsToDraw; i++ )
				{
						glVertex2f( xvalue, *curryvalue );
						xvalue+=mXHopSize;
						curryvalue++;
						glVertex2f( xvalue, *curryvalue );
				}
				
				glEnd();
		}
}

