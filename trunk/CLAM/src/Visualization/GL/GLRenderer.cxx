#include "GLRenderer.hxx"
#include "CLAMGL.hxx"

namespace CLAMVM
{
		GLRenderer::~GLRenderer()
		{
		}

		void GLRenderer::ApplyGLState( )
		{
				glClearColor( 0.0, 0.0, 0.0, 0.0 );
				glShadeModel( GL_FLAT );
				glDisable( GL_DEPTH_TEST );
				glDisable( GL_CULL_FACE );
		}
		
}
