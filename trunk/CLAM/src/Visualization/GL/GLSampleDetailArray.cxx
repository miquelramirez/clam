#include "GLSampleDetailArray.hxx"
#include "CLAMGL.hxx"

namespace CLAMVM
{

		GLSampleDetailArray::GLSampleDetailArray()
		{
		}

		GLSampleDetailArray::~GLSampleDetailArray()
		{
		}

		void GLSampleDetailArray::ExecuteGLCommands()
		{
				glClear( GL_COLOR_BUFFER_BIT );
				glMatrixMode( GL_MODELVIEW );
				glLoadIdentity();

				TData xvalue = mXStart;
				const TData* curryvalue = mValues;
				const TData halfHop = mXHopSize * 0.5f;

				glColor3ubv( mLineColor );
				glBegin( GL_LINE_STRIP );

				glVertex2d( xvalue - halfHop, *curryvalue );

				for ( unsigned i = 0; i < mElemsToDraw; i++ )
				{
					glVertex2d( xvalue + halfHop, *curryvalue );
					*curryvalue++;
					xvalue+=mXHopSize;
					glVertex2d( xvalue - halfHop, *curryvalue );				
				}
				
				glEnd();				
		}
	
}
