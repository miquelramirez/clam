#ifndef __GLRENDERER__
#define __GLRENDERER__

namespace CLAMVM
{
		class GLRenderer
		{
		public:
				virtual void ApplyGLState();
				virtual void ExecuteGLCommands() = 0;
				
				virtual ~GLRenderer();
		};
}

#endif // GLRenderer.hxx
