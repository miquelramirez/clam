#ifndef __VMQT_RENDERER2D_H__
#define __VMQT_RENDERER2D_H__

#include <QString>
#include <QCursor>
#include "vm_renderer.hxx"
#include "vm_data_types.hxx"

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

			void set_enabled(bool enabled);
			void set_grid(Grid* grid);

		signals:
			void requestRefresh();
			void requestUpdate();
			void hZoomRef(double);
			void toolTip(QString);
			void cursorChanged(QCursor);
			void working(QString,bool);

		protected:
			virtual void render()=0;
			virtual void set_xrange(double xmin, double xmax);
			virtual void set_yrange(double ymin, double ymax);
			virtual void set_hbounds(double left, double right);
			virtual void set_vbounds(double bottom, double top);
			virtual void mouse_press_event(double x, double y);
			virtual void mouse_release_event(double x, double y);
			virtual void mouse_move_event(double x, double y);
			virtual void mouse_doubleclick_event(double x, double y);
			virtual void key_press_event(int key);
			virtual void key_release_event(int key);
			virtual void enter_event();
			virtual void leave_event();
			virtual void set_viewport(const GLViewport& v);
			
			void set_key(const QString& key);
			void catch_events(bool ce);

			int get_glList();

			bool    rd_catch_events;
			QString rd_key;

			GLView     rd_view;
			GLViewport rd_viewport;
			Range      rd_xrange;
			Range      rd_yrange;
			Keyboard   rd_keyboard;
			Grid*      rd_grid;
		};
	}
}

#endif

