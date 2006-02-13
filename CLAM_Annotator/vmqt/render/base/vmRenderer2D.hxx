#ifndef __VMQT_RENDERER2D_H__
#define __VMQT_RENDERER2D_H__

#include <QString>
#include <QCursor>
#include "vmRenderer.hxx"
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		class Grid;

		class Renderer2D : public Renderer
		{
			friend class Plot2D;

			Q_OBJECT
		public:
			Renderer2D();
			virtual ~Renderer2D();

			void SetEnabled(bool enabled);
			void SetGrid(Grid* grid);

		signals:
			void requestRefresh();
			void requestUpdate();
			void hZoomRef(double);
			void toolTip(QString);
			void cursorChanged(QCursor);
			void working(QString,bool);

		protected:
			virtual void Render()=0;
			virtual void SetXRange(double xmin, double xmax);
			virtual void SetYRange(double ymin, double ymax);
			virtual void SetHBounds(double left, double right);
			virtual void SetVBounds(double bottom, double top);
			virtual void MousePressEvent(double x, double y);
			virtual void MouseReleaseEvent(double x, double y);
			virtual void MouseMoveEvent(double x, double y);
			virtual void MouseDoubleclickEvent(double x, double y);
			virtual void KeyPressEvent(int key);
			virtual void KeyReleaseEvent(int key);
			virtual void EnterEvent();
			virtual void LeaveEvent();
			virtual void SetViewport(const GLViewport& v);
			
			void SetKey(const QString& key);
			void CatchEvents(bool ce);

			int GetglList();

			bool    mCatchEvents;
			QString mKey;

			GLView     mView;
			GLViewport mViewport;
			Range      mXRange;
			Range      mYRange;
			Keyboard   mKeyboard;
			Grid*      mGrid;
		};
	}
}

#endif

