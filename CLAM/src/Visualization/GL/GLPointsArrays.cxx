#include "GLPointsArrays.hxx"
#include "Assert.hxx"
#include "CLAMGL.hxx"

namespace CLAMVM
{
	GLPointsArrays::GLPointsArrays()
		: mXDataArray( NULL ), mYDataArray( NULL ), mXDataLen( 0 ), mYDataLen( 0 ), mMinY( 0 )
	{
	}

	GLPointsArrays::~GLPointsArrays()
	{
	}

	void GLPointsArrays::ExecuteGLCommands()
	{
		if ( mXDataArray == NULL ) return;
		if ( mYDataArray == NULL ) return;

		CLAM_DEBUG_ASSERT( mXDataLen == mYDataLen, "X's and Y's arrays lenghts differ!!" );
				
		glMatrixMode( GL_MODELVIEW );
				
		glLineWidth( 1.5 );
		for ( int i = 0; i < mXDataLen; i++ )
		{
			glColor3ubv( mLineColor );

			glBegin( GL_LINES );
			{
				glVertex2f( mXDataArray[i], mMinY );
				glVertex2f( mXDataArray[i], mYDataArray[i] );
			}
			glEnd();
			// box at the tip of the peak ( leftbottom - rightbottom - righttop - lefttop )
			glColor3ub( 255, 0, 0 );
			glBegin( GL_LINE_STRIP );
			{
				glVertex2f( mXDataArray[i]-0.05f, mYDataArray[i]-0.05f );
				glVertex2f( mXDataArray[i]+0.05f, mYDataArray[i]-0.05f );
				glVertex2f( mXDataArray[i]+0.05f, mYDataArray[i]+0.05f );
				glVertex2f( mXDataArray[i]-0.05f, mYDataArray[i]+0.05f );
				glVertex2f( mXDataArray[i]-0.05f, mYDataArray[i]-0.05f );
			}
			glEnd();
		}
		glLineWidth( 1.0 );

	}
		
}
