#include "GLMaxMinArrays.hxx"
#include "Assert.hxx"
#include "CLAMGL.hxx"

namespace CLAMVM
{
		GLMaxMinArrays::GLMaxMinArrays()
				: mMaxsPtr( NULL ), mMinsPtr( NULL ), mNElems( 0 ), mMaxY( 1.0 ), mMinY( -1.0 )
		{
		}

		GLMaxMinArrays::~GLMaxMinArrays()
		{
		}

		void GLMaxMinArrays::ExecuteGLCommands()
		{
				CLAM_ASSERT( (mMaxsPtr!=NULL && mNElems>0), "Invalid pointer!" );
				CLAM_ASSERT( (mMinsPtr!=NULL && mNElems>0), "Invalid pointer!" );

				float fnelems = float(mNElems);

				glMatrixMode( GL_PROJECTION );
				glPushMatrix();
				glLoadIdentity();
				glOrtho( 0, fnelems, mMinY, mMaxY, -1, 1 );
				glMatrixMode( GL_MODELVIEW );
				glClear( GL_COLOR_BUFFER_BIT );
				glLoadIdentity();

				glColor3f( 0, 200, 0 );

				glBegin( GL_LINE_STRIP );

				for ( float x = 0.0f; x < fnelems; x+=1.0f )
				{
/*						if ( mMaxsPtr == mMinsPtr )
						{
							glVertex2f( x-1.0, *mMaxsPtr++);
							glVertex2f( x, *mMinsPtr++ );
						}
						else
						{*/
							glVertex2f( x, *mMaxsPtr++ );
							glVertex2f( x, *mMinsPtr++ );
						//}
				}

				glEnd();
				glMatrixMode( GL_PROJECTION );
				glPopMatrix();
				glMatrixMode( GL_MODELVIEW );
		}
}
