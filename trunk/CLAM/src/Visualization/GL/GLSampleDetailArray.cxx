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
				//glClear( GL_COLOR_BUFFER_BIT );
				glMatrixMode( GL_MODELVIEW );
				glLoadIdentity();

				TData xvalue = mXStart;
				const TData halfHop = mXHopSize * 0.5f;

				glColor3ubv( mLineColor );
				glBegin( GL_LINE_STRIP );

				unsigned i = 0;

				for ( i = 0; i < mElemsToDraw; i++ )
				{

					glVertex2d( xvalue - halfHop, mValues[i] );
					glVertex2d( xvalue + halfHop, mValues[i]);
					xvalue+=mXHopSize;
				}

				
				glEnd();				
		}
	
}
