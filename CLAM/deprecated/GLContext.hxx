#ifndef __GLCONTEXT__
#define __GLCONTEXT__

#include <qapplication.h>
#include <qgl.h>
#if defined(Q_WS_X11)
#include <GL/glx.h>
#endif
#if defined(Q_WS_MAC)
#include </System/Library/Frameworks/AGL.framework/Headers/agl.h>
#endif

namespace CLAM
{
	namespace VM
	{
		class GLContext : public QGLContext
		{
		public:
			GLContext(const QGLFormat& fmt, QPaintDevice* dev);

		protected:
			//TODO PA: (refactoring) the platform specific should be hiden into 
			//     the implementation, not affecting the class interface
#if defined(Q_WS_WIN)
			int choosePixelFormat(void* p, HDC hdc);
#elif defined(Q_WS_X11)
			void* chooseVisual();
#elif defined(Q_WS_MAC)
			void* chooseMacVisual(GDHandle gdev);
#endif
		};
	}
}

#endif

