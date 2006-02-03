#include <QResizeEvent>
#include "vm_renderer2d.hxx"
#include "vm_plot2d.hxx"

namespace CLAM
{
	namespace VM
	{

		Plot2D::Plot2D(QWidget* parent)
			: Plot(parent)
			, cp_tooltip("")
			, cp_xzoom_steps(0)
			, cp_yzoom_steps(0)
			, cp_current_xzoom_step(0)
			, cp_current_yzoom_step(0)
			, cp_hzoom_ref(0.0)
			, cp_current_xspan(1.0)
			, cp_current_yspan(1.0)
			, cp_xrange(-1.0,1.0)
			, cp_yrange(-1.0,1.0)
			, cp_viewport(0,0,10,10)
		{
			setMouseTracking(true);
			setAutoBufferSwap(false);
			
			cp_tooltip_font.setFamily("fixed");
			cp_tooltip_font.setPointSize(8);
			cp_tooltip_font.setBold(true);
			cp_tooltip_font.setStyleHint(QFont::Courier);
		}

		Plot2D::~Plot2D()
		{
			clear_renderers();
		}

		void Plot2D::set_xrange(double xmin, double xmax)
		{
			if(xmax <= xmin) return;
			cp_xrange.min = xmin;
			cp_xrange.max = xmax;
			cp_current_xspan = cp_xrange.span();
			cp_view.left = cp_xrange.min;
			cp_view.right = cp_xrange.max;
			Renderers::iterator it = cp_renderers.begin();
			for(; it != cp_renderers.end(); it++) it->second->set_xrange(cp_xrange.min,cp_xrange.max);
			set_renderers_hbounds(cp_xrange.min,cp_xrange.max);
			int sv = get_hscroll_value();
			emit hScrollMaxValue(get_xpixels());
			emit hScrollValue(sv);
			emit xRulerRange(cp_xrange.min,cp_xrange.max);
		}

		void Plot2D::set_yrange(double ymin, double ymax)
		{
			if(ymax <= ymin) return;
			cp_yrange.min = ymin;
			cp_yrange.max = ymax;
			cp_current_yspan = cp_yrange.span();
			cp_view.bottom = cp_yrange.min;
			cp_view.top = cp_yrange.max;
			Renderers::iterator it = cp_renderers.begin();
			for(; it != cp_renderers.end(); it++) it->second->set_yrange(cp_yrange.min,cp_yrange.max);
			set_renderers_vbounds(cp_yrange.min,cp_yrange.max);
			int sv = get_vscroll_value();
			emit vScrollMaxValue(get_ypixels());
			emit vScrollValue(sv);
			emit yRulerRange(cp_yrange.min,cp_yrange.max);
		}
	
		void Plot2D::set_zoom_steps(int xsteps, int ysteps)
		{
			if(xsteps < 0 || ysteps < 0) return;
			cp_xzoom_steps = xsteps;
			cp_yzoom_steps = ysteps;
			cp_current_xzoom_step = cp_xzoom_steps;
			cp_current_yzoom_step = cp_yzoom_steps;
			int xratio = 1;
			int yratio = 1;
			for(int i=0; i < cp_xzoom_steps; i++) xratio *= 2;
			for(int i=0; i < cp_yzoom_steps; i++) yratio *= 2;
			emit hZoomRatio("1:"+QString::number(xratio));
			emit vZoomRatio("1:"+QString::number(yratio));
			set_xrange(cp_xrange.min,cp_xrange.max);
			set_yrange(cp_yrange.max,cp_yrange.min);
		}

		bool Plot2D::add_renderer(const QString& key, Renderer2D* renderer)
		{
			if(exist_renderer(key)) return false;
			cp_renderers[key] = renderer;
			Renderer2D* rd = get_renderer(key);
			if(!rd) return false;
			connect(rd,SIGNAL(requestUpdate()),this,SLOT(updateRenderers()));
			connect(rd,SIGNAL(requestRefresh()),this,SLOT(updateGL()));
			connect(rd,SIGNAL(hZoomRef(double)),this,SLOT(updateHZoomRef(double)));
			connect(rd,SIGNAL(toolTip(QString)),this,SLOT(setToolTip(QString)));
			connect(rd,SIGNAL(cursorChanged(QCursor)),this,SLOT(changeCursor(QCursor)));
			connect(rd,SIGNAL(working(QString,bool)),this,SLOT(rendererWorking(QString,bool)));
			rd->set_xrange(cp_xrange.min,cp_xrange.max);
			rd->set_yrange(cp_yrange.min,cp_yrange.max);
			rd->set_key(key);
			cp_draw_order.push_back(key);
			return true;
		}

		bool Plot2D::set_renderer_enabled(const QString& key, bool enabled)
		{
			if(!exist_renderer(key)) return false;
			cp_renderers[key]->set_enabled(enabled);
			return true;
		}
	
		Renderer2D* Plot2D::get_renderer(const QString& key)
		{
			if(!exist_renderer(key)) return 0;
			return cp_renderers[key];
		}

		void Plot2D::hZoomIn()
		{	
			if(!cp_current_xzoom_step) return;
			cp_current_xspan /= 2.0;
			update_hbounds(true);
			cp_current_xzoom_step--;
			int xratio=1;
			for(int i=0; i < cp_current_xzoom_step; i++) xratio *= 2;
			emit hZoomRatio("1:"+QString::number(xratio));
			emit hScrollMaxValue(get_xpixels());
			emit hScrollValue(get_hscroll_value());
		}

		void Plot2D::hZoomOut()
		{
			if(cp_current_xzoom_step == cp_xzoom_steps) return;
			cp_current_xspan *= 2.0;
			update_hbounds(false);
			cp_current_xzoom_step++;
			int xratio=1;
			for(int i=0; i < cp_current_xzoom_step; i++) xratio *= 2;
			emit hZoomRatio("1:"+QString::number(xratio));
			emit hScrollValue(get_hscroll_value());
			emit hScrollMaxValue(get_xpixels());
		}

		void Plot2D::vZoomIn()
		{
			if(!cp_current_yzoom_step) return;
			cp_current_yspan /= 2.0;
			update_vbounds(true);
			cp_current_yzoom_step--;
			int yratio=1;
			for(int i=0; i < cp_current_yzoom_step; i++) yratio *= 2;
			emit vZoomRatio("1:"+QString::number(yratio));
			emit vScrollMaxValue(get_ypixels());
			emit vScrollValue(get_vscroll_value());
		}

		void Plot2D::vZoomOut()
		{
			if(cp_current_yzoom_step == cp_yzoom_steps) return;
			cp_current_yspan *= 2.0;
			update_vbounds(false);
			cp_current_yzoom_step++;
			int yratio=1;
			for(int i=0; i < cp_current_yzoom_step; i++) yratio *= 2;
			emit vZoomRatio("1:"+QString::number(yratio));
			emit vScrollValue(get_vscroll_value());
			emit vScrollMaxValue(get_ypixels()); 
		}
		
		void Plot2D::updateHScrollValue(int value)
		{
			double left = cp_xrange.span()/double(get_xpixels())*double(value)+cp_xrange.min;
			double right = left+cp_current_xspan;
			set_hbounds(left,right);
}

		void Plot2D::updateVScrollValue(int value)
		{
			double bottom = cp_yrange.span()/double(get_ypixels())*double(value)+cp_yrange.min;
			double top = bottom+cp_current_yspan;
			double tmp = bottom;
			bottom = cp_yrange.min+(cp_yrange.max-top);
			top = cp_yrange.max-(tmp-cp_yrange.min);
			set_vbounds(bottom,top);
		}

		void Plot2D::setHBounds(double left, double right)
		{
			set_hbounds(left,right);
		}

		void Plot2D::setVBounds(double bottom, double top)
		{
			set_vbounds(bottom,top);
		}

		void Plot2D::paintGL()
		{
			if(cp_do_resize)
			{
				glViewport(0,0,cp_viewport.w,cp_viewport.h);
				cp_do_resize = false;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(cp_view.left,cp_view.right,cp_view.bottom,cp_view.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glShadeModel(GL_FLAT);
			glClearColor(double(cp_bg_color.r)/255.0, 
						 double(cp_bg_color.g)/255.0, 
						 double(cp_bg_color.b)/255.0, 
						 double(cp_bg_color.a)/255.0);
			glClear(GL_COLOR_BUFFER_BIT);
			draw();
			render_tooltip(); 
			swapBuffers();
		}
		
		void Plot2D::resizeEvent(QResizeEvent* e)
		{
			cp_viewport.w = e->size().width();
			cp_viewport.h = e->size().height();
			cp_do_resize = true;
			Renderers::iterator it = cp_renderers.begin();
			for(; it != cp_renderers.end(); it++) it->second->set_viewport(cp_viewport);
			int sv = get_hscroll_value();
			emit hScrollMaxValue(get_xpixels());
			emit hScrollValue(sv);

			sv = get_vscroll_value();
			emit vScrollMaxValue(get_ypixels());
			emit vScrollValue(sv);
	
			emit xRulerRange(cp_view.left,cp_view.right);
			emit yRulerRange(cp_view.bottom,cp_view.top);
		}

		void Plot2D::mouseMoveEvent(QMouseEvent* e)
		{
			cp_mouse_pos = std::make_pair(e->x(),e->y());
			std::pair<double,double> coords = GetXY(cp_mouse_pos.first,cp_mouse_pos.second);
			Renderers::iterator it = cp_renderers.begin();
			for(;it != cp_renderers.end(); it++) it->second->mouse_move_event(coords.first,coords.second);
		}

		void Plot2D::mousePressEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = cp_renderers.begin();
			for(;it != cp_renderers.end(); it++) it->second->mouse_press_event(coords.first,coords.second);
		}

		void Plot2D::mouseReleaseEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = cp_renderers.begin();
			for(;it != cp_renderers.end(); it++) it->second->mouse_release_event(coords.first,coords.second);
		}

		void Plot2D::mouseDoubleClickEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = cp_renderers.begin();
			for(;it != cp_renderers.end(); it++) it->second->mouse_doubleclick_event(coords.first,coords.second);
		}

		void Plot2D::keyPressEvent(QKeyEvent * e)
		{
			Renderers::iterator it = cp_renderers.begin();
			for(;it != cp_renderers.end(); it++) it->second->key_press_event(e->key());
		}

		void Plot2D::keyReleaseEvent(QKeyEvent * e)
		{
			Renderers::iterator it = cp_renderers.begin();
			for(;it != cp_renderers.end(); it++) it->second->key_release_event(e->key());
		}

		void Plot2D::enterEvent(QEvent* e)
		{
			Renderers::iterator it = cp_renderers.begin();
			for(;it != cp_renderers.end(); it++) it->second->enter_event();
			QGLWidget::enterEvent(e);
		}

		void Plot2D::leaveEvent(QEvent* e)
		{
			Renderers::iterator it = cp_renderers.begin();
			for(;it != cp_renderers.end(); it++) it->second->leave_event();
			QGLWidget::leaveEvent(e);
		}

		void Plot2D::updateRenderers()
		{
			set_renderers_hbounds(cp_view.left,cp_view.right);
			set_renderers_vbounds(cp_view.bottom,cp_view.top);
			updateGL();
		}

		void Plot2D::updateHZoomRef(double ref)
		{
			if(ref < cp_xrange.min || ref > cp_xrange.max) return;
			cp_hzoom_ref = ref;
		}

		void Plot2D::changeCursor(QCursor cursor)
		{
			setCursor(cursor);
		}

		void Plot2D::setToolTip(QString str)
		{
			cp_tooltip = str;
			update();
		}

		void Plot2D::rendererWorking(QString key,bool working)
		{
			Renderers::iterator it = cp_renderers.begin();
			if(working)
			{
				for(;it != cp_renderers.end(); it++)
				{
					if(it->first == key) continue;
					it->second->catch_events(false);
				}
			}
			else
			{
				for(;it != cp_renderers.end(); it++)
				{
					it->second->catch_events(true);
				}

			}
		}

		void Plot2D::draw()
		{
			std::vector<QString>::iterator it = cp_draw_order.begin();
			for(; it != cp_draw_order.end(); it++) cp_renderers[(*it)]->render();
		}

		void Plot2D::render_tooltip()
		{
			if(cp_tooltip.isEmpty()) return;

			QRect rect = tooltip_rect();
			QFontMetrics font_metrics(cp_tooltip_font);
			int font_height = font_metrics.height();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0,cp_viewport.w,cp_viewport.h,0.0,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glColor3ub(255,255,0);
			glBegin(GL_QUADS);
			glVertex2d(rect.left(),rect.top());
			glVertex2d(rect.left()+rect.width(),rect.top());
			glVertex2d(rect.left()+rect.width(),rect.bottom());
			glVertex2d(rect.left(),rect.bottom());
			glEnd();
			glLineWidth(1);
			glColor3ub(0,0,0);
			glBegin(GL_LINE_STRIP);
			glVertex2d(rect.left(),rect.top());
			glVertex2d(rect.left()+rect.width(),rect.top());
			glVertex2d(rect.left()+rect.width(),rect.bottom());
			glVertex2d(rect.left(),rect.bottom());
			glVertex2d(rect.left(),rect.top());
			glEnd();
   
			renderText(rect.left()+5,rect.top()+font_height+2,cp_tooltip,cp_tooltip_font);

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void Plot2D::clear_renderers()
		{
			Renderers::iterator it = cp_renderers.begin();
			for(; it != cp_renderers.end(); it++) delete it->second;
				
			cp_renderers.clear();
		}

		bool Plot2D::exist_renderer(const QString& key)
		{
			return (cp_renderers.find(key) != cp_renderers.end());
		}

		void Plot2D::set_hbounds(double left, double right)
		{
			if(left==cp_view.left && right==cp_view.right) return;
			cp_view.left = left;
			cp_view.right = right;
			set_renderers_hbounds(cp_view.left,cp_view.right);
			emit xRulerRange(cp_view.left, cp_view.right);
			updateGL();
		}

		void Plot2D::set_vbounds(double bottom, double top)
		{
			if(bottom==cp_view.bottom && top==cp_view.top) return;
			cp_view.bottom = bottom;
			cp_view.top = top;
			set_renderers_vbounds(cp_view.bottom,cp_view.top);
			emit yRulerRange(cp_view.bottom, cp_view.top);
			updateGL();
		}

		void Plot2D::update_hbounds(bool zin)
		{
			double left = cp_view.left;
			double right = cp_view.right;
			if(zin)
			{
				if(reference_is_visible())
				{
					double ref = get_reference();
					if(ref-cp_xrange.min >= cp_current_xspan/2.0)
					{
						left = (ref+cp_current_xspan/2.0 < cp_xrange.span()) 
							? ref-cp_current_xspan/2.0 : ref-(cp_current_xspan-(cp_xrange.span()-ref));
					}
					right = left+cp_current_xspan;
				}
				else
				{
					left += cp_current_xspan/2.0;
					right -= cp_current_xspan/2.0;
				}
			}
			else
			{
				left -= cp_current_xspan/4.0;
				right += cp_current_xspan/4.0;
				if(left < cp_xrange.min)
				{
					left = cp_xrange.min;
					right = left+cp_current_xspan;
				}
				if(right > cp_xrange.max)
				{
					right = cp_xrange.max;
					left = right-cp_current_xspan;
				}
			}
			set_hbounds(left,right);
		}

		void Plot2D::update_vbounds(bool zin)
		{
			double bottom = cp_view.bottom;
			double top = cp_view.top;
			if(zin)
			{
				bottom += cp_current_yspan/2.0;
				top -= cp_current_yspan/2.0;
			}
			else
			{
				bottom -= cp_current_yspan/4.0;
				top += cp_current_yspan/4.0;
				if(bottom < cp_yrange.min)
				{
					bottom = cp_yrange.min;
					top = bottom+cp_current_yspan;
				}
				if(top > cp_yrange.max)
				{
					top = cp_yrange.max;
					bottom = top-cp_current_yspan;
				}
			}
			set_vbounds(bottom,top);
		}

		int Plot2D::get_xpixels() 
		{
			return int(cp_xrange.span()*double(cp_viewport.w)/cp_current_xspan);
		}
		
		int Plot2D::get_ypixels() 
		{
			return int(cp_yrange.span()*double(cp_viewport.h)/cp_current_yspan);
		}

		int Plot2D::get_hscroll_value() 
		{
			return int((cp_view.left-cp_xrange.min)*double(get_xpixels())/cp_xrange.span());
		}

		int Plot2D::get_vscroll_value() 
		{
			return int((cp_yrange.max-cp_view.top)*double(get_ypixels())/cp_yrange.span());
		}

		bool Plot2D::reference_is_visible() const
		{
			return (cp_hzoom_ref >= cp_view.left && cp_hzoom_ref <= cp_view.right);
		}

		double Plot2D::get_reference() const
		{
			return cp_hzoom_ref;
		}

		void Plot2D::set_renderers_hbounds(double left, double right)
		{
			Renderers::iterator it = cp_renderers.begin();
			for(; it != cp_renderers.end(); it++) it->second->set_hbounds(left,right); 
		}

		void Plot2D::set_renderers_vbounds(double bottom, double top)
		{
			Renderers::iterator it = cp_renderers.begin();
			for(; it != cp_renderers.end(); it++) it->second->set_vbounds(bottom,top);
		}

		std::pair<double,double> Plot2D::GetXY(int x, int y)
		{
			double xcoord = double(x);
			xcoord *= (cp_view.right-cp_view.left);
			xcoord /= double(cp_viewport.w);
			xcoord += cp_view.left;
			double ycoord = double(-y+cp_viewport.h);
			ycoord *= (cp_view.top-cp_view.bottom);
			ycoord /= double(cp_viewport.h);
			ycoord += cp_view.bottom;
			return std::make_pair(xcoord,ycoord);
		}

		QRect Plot2D::tooltip_rect()
		{
			QFontMetrics font_metrics(cp_tooltip_font);
			
			int x = cp_mouse_pos.first+2;
			int y = cp_mouse_pos.second+2;

			int w = font_metrics.width(cp_tooltip)+10;
			int h = font_metrics.height()+10;

			if(x+w > cp_viewport.w) x -= w;
			if(y+h > cp_viewport.h) y -= h;

			return QRect(x,y,w,h);
		}

		void Plot2D::bring_to_front(const QString& key)
		{
			if(cp_draw_order.size() <= 1) return;
			if(cp_draw_order[cp_draw_order.size()-1] == key) return;
			unsigned i=0;
			for(; i < cp_draw_order.size(); i++) if(cp_draw_order[i]==key) break;
			if(i==cp_draw_order.size()) return;
			std::vector<QString> tmp;
			for(unsigned k=0; k < cp_draw_order.size(); k++)
			{
				if(k==i) continue;
				tmp.push_back(cp_draw_order[k]);
			}
			for(i=0; i < tmp.size(); i++) cp_draw_order[i]=tmp[i];
			cp_draw_order[cp_draw_order.size()-1] = key;

		}

		void Plot2D::send_to_back(const QString& key)
		{
			if(cp_draw_order.size() <= 1) return;
			if(cp_draw_order[0] == key) return;
			unsigned i=0;
			for(; i < cp_draw_order.size(); i++) if(cp_draw_order[i]==key) break;
			if(i==cp_draw_order.size()) return;
			std::vector<QString> tmp;
			for(unsigned k=0; k < cp_draw_order.size(); k++)
			{
				if(k==i) continue;
				tmp.push_back(cp_draw_order[k]);
			}
			for(i=1; i < cp_draw_order.size(); i++) cp_draw_order[i]=tmp[i-1];
			cp_draw_order[0] = key;
		}

	}
}

// END
