#ifndef _VMQT__PLOT2D_H__
#define __VMQT_PLOT2D_H__

#include <map>
#include <vector>
#include <QString>
#include <QCursor>
#include "vm_plot.hxx"

namespace CLAM
{
	namespace VM
	{

		class CLAM::VM::Renderer2D;

		class Plot2D : public CLAM::VM::Plot
		{
			typedef std::map<QString, CLAM::VM::Renderer2D*> Renderers;
			
			Q_OBJECT
		public:
			Plot2D(QWidget* parent=0);
			~Plot2D();

			void set_xrange(double xmin, double xmax);
			void set_yrange(double ymin, double ymax);
	
			void set_zoom_steps(int xsteps, int ysteps);
  
			bool add_renderer(const QString& key, CLAM::VM::Renderer2D* renderer);
			bool set_renderer_enabled(const QString& key, bool enabled);
	
			CLAM::VM::Renderer2D* get_renderer(const QString& key);

			void bring_to_front(const QString& key);
			void send_to_back(const QString& key);

		signals:
			void hZoomRatio(QString);
			void vZoomRatio(QString);
			void hScrollValue(int);
			void vScrollValue(int);
			void hScrollMaxValue(int);
			void vScrollMaxValue(int);
			void xRulerRange(double,double);
			void yRulerRange(double,double);
		   
		public slots:
			void hZoomIn();
			void hZoomOut();	
			void vZoomIn();
			void vZoomOut();
			void updateHScrollValue(int);
			void updateVScrollValue(int);
			void setHBounds(double, double);
			void setVBounds(double, double);

		protected:
			void paintGL(); 
			void resizeEvent(QResizeEvent* e);
			void mouseMoveEvent(QMouseEvent* e);
			void mousePressEvent(QMouseEvent* e);
			void mouseReleaseEvent(QMouseEvent* e);
			void mouseDoubleClickEvent(QMouseEvent* e);
			void keyPressEvent(QKeyEvent * e);
			void keyReleaseEvent(QKeyEvent * e);
			void enterEvent(QEvent* e);
			void leaveEvent(QEvent* e);

		private slots:
			void updateRenderers();
			void updateHZoomRef(double);
			void changeCursor(QCursor);
			void setToolTip(QString);
			void rendererWorking(QString,bool);

		private:
			Renderers cp_renderers;
			QString   cp_tooltip;
			QFont     cp_tooltip_font;

			int    cp_xzoom_steps;
			int    cp_yzoom_steps;
			int    cp_current_xzoom_step;
			int    cp_current_yzoom_step;
			double cp_hzoom_ref;
			double cp_current_xspan;
			double cp_current_yspan;

			CLAM::VM::Range cp_xrange;
			CLAM::VM::Range cp_yrange;

			CLAM::VM::GLView     cp_view;
			CLAM::VM::GLViewport cp_viewport;

			std::pair<int,int>   cp_mouse_pos;
			std::vector<QString> cp_draw_order;

			void draw();
			void render_tooltip();
			void clear_renderers();
			bool exist_renderer(const QString& key);

			void set_hbounds(double left, double right);
			void set_vbounds(double bottom, double top);

			void update_hbounds(bool zin);
			void update_vbounds(bool zin);

			int get_xpixels();
			int get_ypixels();
			int get_hscroll_value();
			int get_vscroll_value();

			bool reference_is_visible() const;
			double get_reference() const;

			void set_renderers_hbounds(double left, double right);
			void set_renderers_vbounds(double bottom, double top);
			std::pair<double,double> GetXY(int x, int y);
			QRect tooltip_rect();
		};
	}
}

#endif
