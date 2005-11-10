#include "GLContext.hxx"

namespace CLAM
{
	namespace VM
	{
		GLContext::GLContext(const QGLFormat& fmt, QPaintDevice* dev)
			: QGLContext(fmt, dev)
		{
		}

#if defined(Q_WS_WIN)
		int GLContext::choosePixelFormat(void* p, HDC hdc)
		{
			PIXELFORMATDESCRIPTOR* pfd = (PIXELFORMATDESCRIPTOR *)p;
			int pfiMax = DescribePixelFormat(hdc, 0, 0, NULL);
			int pfi;
			for(pfi=1; pfi <= pfiMax; pfi++)
			{
				DescribePixelFormat(hdc, pfi, sizeof(PIXELFORMATDESCRIPTOR), pfd);
				if(pfd->cDepthBits == 32) return pfi;
			}
			pfi = QGLContext::choosePixelFormat(pfd,hdc);
			qWarning("32-bit depth unavailable: using %d bits", pfd->cDepthBits);
			return pfi;
		}
#endif

#if defined(Q_WS_X11)
		void* GLContext::chooseVisual()
		{
			GLint attribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 32, None};
			XVisualInfo* vis = glXChooseVisual(device()->x11Display(), device()->x11Screen(), attribs);
			if(vis)
			{
				GLint depth = 0;
				glXGetConfig(device()->x11Display(), vis, GLX_DEPTH_SIZE, &depth);
				if(depth != 32)
				{
					qWarning("32-bit depth unavailable: using %d bits", depth);
				}
				return vis;
			}
			return QGLContext::chooseVisual();
		}
#endif

#if defined(Q_WS_MAC)
		void* GLContext::chooseMacVisual(GDHandle gdev)
		{
			GLint attribs[] = {AGL_ALL_RENDERERS, AGL_RGBA, AGL_DEPTH_SIZE, 32, AGL_NONE};
			AGLPixelFormat fmt = aglChoosePixelFormat(NULL, 0, attribs);
			if(fmt)
			{
				GLint depth;
				aglDescribePixelFormat(fmt, AGL_DEPTH_SIZE, &depth);
				if(depth != 32)
				{
					qWarning("32-bit depth unavailable: using %d bits", int(depth));
				}
				return fmt;
			}
			return QGLContext::chooseMacVisual(gdev);
		}
#endif

	}
}

// END

