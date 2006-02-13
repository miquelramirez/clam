#include "CLAMGL.hxx"
#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{

		Renderer2D::Renderer2D()
			: mCatchEvents(true)
			, mViewport(-1,-1,1,1)
			, mGrid(0)
		{
		}

		Renderer2D::~Renderer2D()
		{
		}

		void Renderer2D::SetEnabled(bool enabled)
		{
			Renderer::SetEnabled(enabled);
			emit requestRefresh();
		}

		void Renderer2D::SetGrid(Grid* grid)
		{
			mGrid = grid;
		}

		void Renderer2D::SetXRange(double xmin, double xmax)
		{
			if(xmax <= xmin) return;
			mXRange.min = xmin;
			mXRange.max = xmax;
			emit requestUpdate();
		}

		void Renderer2D::SetYRange(double ymin, double ymax)
		{
			if(ymax <= ymin) return;
			mYRange. min = ymin;
			mYRange.max = ymax;
			emit requestUpdate();
		}

		void Renderer2D::SetHBounds(double left, double right)
		{
			mView.left = left;
			mView.right = right;
		}

		void Renderer2D::SetVBounds(double bottom, double top)
		{
			mView.bottom = bottom;
			mView.top = top;
		}

		void Renderer2D::MousePressEvent(double x, double y)
		{
			// do nothing by default
		}

		void Renderer2D::MouseReleaseEvent(double x, double y)
		{
			// do nothing by default
		}

		void Renderer2D::MouseMoveEvent(double x, double y)
		{
			// do nothing by default
		}

		void Renderer2D::MouseDoubleclickEvent(double x, double y)
		{
			// do nothing by default
		}

		void Renderer2D::KeyPressEvent(int key)
		{
			// do nothing by default
		}

		void Renderer2D::KeyReleaseEvent(int key)
		{
			// do nothing by default
		}

		void Renderer2D::EnterEvent()
		{
			// do nothing by default
		}

		void Renderer2D::LeaveEvent()
		{
			// do nothing by default
		}

		void Renderer2D::SetViewport(const GLViewport& v)
		{
			mViewport = v;
		}

		void Renderer2D::SetKey(const QString& key)
		{
			mKey = key;
		}

		void Renderer2D::CatchEvents(bool ce)
		{
			mCatchEvents = ce;
		}

		int Renderer2D::GetglList()
		{
			int gl_list = 1;
			while(glIsList(gl_list)) gl_list++;
			return gl_list;
		}
	}
}

// END
