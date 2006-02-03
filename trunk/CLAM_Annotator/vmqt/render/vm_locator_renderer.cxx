#include "CLAMGL.hxx"
#include "vm_locator_renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		Locator::Locator()
			: rd_updating_locator(false)
			, rd_locator_pos(-1E6)
			, rd_region(0.0,0.0)
			, rd_locator_color(0,0,0)
			, rd_region_color(185,185,185)
			, rd_edition_mode(Idle)
		{
		}

		Locator::~Locator()
		{
		}

		void Locator::set_locator_color(const Color& c)
		{
			rd_locator_color = c;
		}

		void Locator::set_region_color(const Color& c)
		{
			rd_region_color = c;
		}

		void Locator::updateLocator(double value)
		{
			if(!rd_updating_locator) rd_updating_locator = true;
			rd_locator_pos = value;
			emit requestRefresh();
		}

		void Locator::updateLocator(double value, bool flag)
		{
			rd_updating_locator = false;
			rd_locator_pos = (flag) ? value : rd_region.min;
			double zoom_ref;
			if(rd_region.min < rd_region.max)
			{
				if(flag)
				{
					rd_region.min = rd_locator_pos;
					emit selectedRegion(rd_region.min,rd_region.max);
				}
				zoom_ref = rd_region.min+rd_region.span()/2.0;
			}
			else
			{
				if(flag) 
				{
					rd_region.min = rd_region.max = rd_locator_pos;
					emit selectedRegion(rd_locator_pos,rd_xrange.max);
				}
				zoom_ref = rd_locator_pos;
			}
			emit hZoomRef(zoom_ref);
			emit requestRefresh();
		}

		void Locator::updateRegion(double begin, double end, bool isEnd)
		{
			if(begin==end)
			{
				rd_locator_pos = begin;
				rd_region.min = rd_region.max = rd_locator_pos;
				emit hZoomRef(rd_locator_pos);
			}
			else
			{
				rd_region.min = begin;
				rd_region.max = end;
				(isEnd) ? rd_locator_pos = end : rd_locator_pos = begin;
				emit hZoomRef(rd_region.min+rd_region.span()/2.0);
			}
			emit requestRefresh();
		}

		void Locator::render()
		{
			if(!rd_enabled) return;
			draw_region(draw_region_mode());
			draw_locator();
		}

		void Locator::set_xrange(double xmin, double xmax)
		{
			Renderer2D::set_xrange(xmin,xmax);
			rd_locator_pos = rd_xrange.min;
		}

		void Locator::mouse_press_event(double x, double y)
		{
			if(!rd_enabled || !rd_catch_events) return;
			if(rd_updating_locator) return;
			if(x == rd_region.min && x == rd_region.max) return;		
			if(rd_keyboard.key_shift && rd_keyboard.key_r)
			{
				if(rd_region.min != rd_region.max)
				{
					double ref = rd_region.min+rd_region.span()/2.0;
					if(x <= ref) 
					{
						rd_region.min = x;  
						rd_locator_pos = rd_region.min;
					}
					else
					{
						rd_region.max = x;
						rd_locator_pos = rd_region.max;
					}
				}
				else
				{
					if(x < rd_region.min)  
					{
						rd_region.min = x;
						rd_locator_pos = rd_region.min;
					}
					else
					{
						rd_region.max = x;
						rd_locator_pos = rd_region.max;
					}
				}
				emit regionChanged(rd_region.min,rd_region.max,rd_locator_pos > rd_region.min);
				emit hZoomRef(rd_region.min+rd_region.span()/2.0);
				emit requestRefresh();
				return;
			}
			if(rd_keyboard.key_r)
			{
				rd_locator_pos = x;
				rd_region.min = rd_region.max = rd_locator_pos;
				emit regionChanged(rd_region.min,rd_region.max,false);
				emit hZoomRef(rd_locator_pos);
				emit requestRefresh();
				return;
			}
			double tolerance = double(TOLERANCE)*(rd_view.right-rd_view.left)/double(rd_viewport.w);
			if(rd_region.min == rd_region.max) 
			{
				if(pick_begin_region(x,tolerance))
				{
					rd_edition_mode = DraggingLocator;
				}
				return;
			}
			if(pick_begin_region(x,tolerance)) 
			{
				rd_edition_mode = DraggingBeginRegion;
				return;
			}
			if(pick_end_region(x,tolerance)) 
			{
				rd_edition_mode = DraggingEndRegion;
				return;
			}
		}

		void Locator::mouse_release_event(double x, double y)
		{
			if(!rd_enabled || !rd_catch_events) return;
			if(rd_updating_locator) return;
			rd_edition_mode = Idle;
			emit working(rd_key,false);
			emit cursorChanged(QCursor(Qt::ArrowCursor));
			double zoom_ref = (rd_region.min == rd_region.max) ? rd_locator_pos : rd_region.min+rd_region.span()/2.0;
			emit hZoomRef(zoom_ref);
			(zoom_ref == rd_locator_pos) 
				? emit selectedRegion(rd_locator_pos,rd_xrange.max) 
				: emit selectedRegion(rd_region.min,rd_region.max);
		}

		void Locator::mouse_move_event(double x, double y)
		{
			if(!rd_enabled || !rd_catch_events) return;
			if(rd_updating_locator) return;
			QString ttip("");
			if(rd_keyboard.key_r) 
			{
				rd_edition_mode = Idle;
				emit working(rd_key,true);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::ArrowCursor));
				return;
			}
			switch (rd_edition_mode)
			{
				case DraggingBeginRegion:
				{
					if(x < rd_xrange.min) x = rd_xrange.min;
					if(x > rd_xrange.max) x = rd_xrange.max;
					bool flag = rd_region.min == rd_locator_pos;
					rd_region.min = x;
					if(rd_region.min >= rd_region.max) rd_region.min = rd_region.max;
					if(rd_region.min < rd_xrange.min) rd_region.min = rd_xrange.min;
					if(flag)
						rd_locator_pos = rd_region.min;
					else
						if(rd_locator_pos <= rd_region.min) rd_locator_pos = rd_region.min;
					ttip = "x:"+QString::number(rd_region.min,'f',2);
					emit toolTip(ttip);
					emit regionChanged(rd_region.min,rd_region.max,rd_locator_pos > rd_region.min);
					emit requestRefresh();	
					return;
				}
				case DraggingEndRegion:
				{
					if(x < rd_xrange.min) x = rd_xrange.min;
					if(x > rd_xrange.max) x = rd_xrange.max;
					bool flag = rd_region.max == rd_locator_pos;
					rd_region.max = x;
					if(rd_region.max <= rd_region.min) rd_region.max = rd_region.min;
					if(rd_region.max > rd_xrange.max) rd_region.max = rd_xrange.max;
					if(flag)
						rd_locator_pos = rd_region.max;
					else
						if(rd_locator_pos >= rd_region.max) rd_locator_pos = rd_region.max;
					ttip = "x:"+QString::number(rd_region.max,'f',2);
					emit toolTip(ttip);
					emit regionChanged(rd_region.min,rd_region.max,rd_locator_pos > rd_region.min);
					emit requestRefresh();	
					return;
				}
				case DraggingLocator:
				{
					if(x < rd_xrange.min) x = rd_xrange.min;
					if(x > rd_xrange.max) x = rd_xrange.max;
					rd_region.min = rd_region.max = rd_locator_pos = x;
					ttip = "x:"+QString::number(rd_locator_pos,'f',2);
					emit toolTip(ttip);
					emit regionChanged(rd_locator_pos,rd_locator_pos,false);
					return;
				}
				default:
					emit working(rd_key,false);
					emit toolTip(ttip);
					emit cursorChanged(QCursor(Qt::ArrowCursor));
					break;
			}
			double tolerance = double(TOLERANCE)*(rd_view.right-rd_view.left)/double(rd_viewport.w);
			if(rd_region.min == rd_region.max) 
			{
				if(pick_begin_region(x,tolerance))
				{
					emit working(rd_key,true);
					ttip = "x:"+QString::number(rd_locator_pos,'f',2);
					emit toolTip(ttip);
					emit cursorChanged(QCursor(Qt::SizeHorCursor));
				}
				else
				{
					rd_edition_mode = Idle;
					emit working(rd_key,false);
					emit toolTip(ttip);
					emit cursorChanged(QCursor(Qt::ArrowCursor));
				}
				return;
			}
			if(pick_begin_region(x,tolerance)) 
			{
				emit working(rd_key,true);
				ttip = "x:"+QString::number(rd_region.min,'f',2);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			if(pick_end_region(x,tolerance)) 
			{
				emit working(rd_key,true);
				ttip = "x:"+QString::number(rd_region.max,'f',2);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
		}

		void Locator::key_press_event(int key)
		{
			if(!rd_enabled || !rd_catch_events) return;
			if(rd_updating_locator) return;
			switch(key)
			{
				case Qt::Key_Shift:
					rd_keyboard.key_shift = true;
					break;
				case Qt::Key_R:
					rd_keyboard.key_r = true;
					emit working(rd_key,true);
					break;
				default:
					break;
			}
		}

		void Locator::key_release_event(int key)
		{
			if(!rd_enabled || !rd_catch_events) return;
			if(rd_updating_locator) return;
			switch(key)
			{
				case Qt::Key_Shift:
					rd_keyboard.key_shift = false;
					break;
				case Qt::Key_R:
					rd_keyboard.key_r = false;
					break;
				default:
					break;
			}
		}

		void Locator::leave_event()
		{
			emit toolTip("");
		}

		int Locator::draw_region_mode()
		{
			// 0 -> neither the points nor the region are visible
			// 1 -> the region is visible but the points are not  
			// 2 -> the first point is visible but the second is not
			// 3 -> the second point is visible but the first is not
			// 4 -> both points are visible
			double p1 = rd_region.min;
			double p2 = rd_region.max;
			if(p1 < rd_view.left && p2 > rd_view.right) return 1;
			if(p1 >= rd_view.left && p2 > rd_view.right) return 2;
			if(p1 < rd_view.left && p2 <= rd_view.right) return 3;
			if(p1 >= rd_view.left && p2 <= rd_view.right) return 4;
			return 0;
		}

		void Locator::draw_region(int mode)
		{
			if(rd_region.min == rd_region.max) return;
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glColor4ub(rd_region_color.r,rd_region_color.g,rd_region_color.b,175);
			glBegin(GL_QUADS);
			switch(mode)
			{
				case 1:
					// the region is visible but the points are not
					glVertex2d(rd_view.left,rd_view.top);
					glVertex2d(rd_view.right,rd_view.top);
					glVertex2d(rd_view.right,rd_view.bottom);
					glVertex2d(0.0f,rd_view.bottom);
					break;
				case 2:
					// the first point is visible but the second is not
					glVertex2d(rd_region.min,rd_view.top);
					glVertex2d(rd_view.right,rd_view.top);
					glVertex2d(rd_view.right,rd_view.bottom);
					glVertex2d(rd_region.min,rd_view.bottom);
					break;
				case 3:
					// the second point is visible but the first is not
					glVertex2d(rd_view.left,rd_view.top);
					glVertex2d(rd_region.max,rd_view.top);
					glVertex2d(rd_region.max,rd_view.bottom);
					glVertex2d(rd_view.left,rd_view.bottom);
					break;
				case 4:
					// both points are visible
					glVertex2d(rd_region.min,rd_view.top);
					glVertex2d(rd_region.max,rd_view.top);
					glVertex2d(rd_region.max,rd_view.bottom);
					glVertex2d(rd_region.min,rd_view.bottom);
					break;
				default:
					// neither the points nor the region are visible
					break;
			}
			glEnd();
			glDisable(GL_BLEND);
		}

		void Locator::draw_locator()
		{
			if(rd_locator_pos < rd_view.left || rd_locator_pos > rd_view.right) return;
			glLineWidth(2);
			glColor3ub(rd_locator_color.r,rd_locator_color.g,rd_locator_color.b);
			glBegin(GL_LINES);
			glVertex2d(rd_locator_pos,rd_view.top);
			glVertex2d(rd_locator_pos,rd_view.bottom);
			glEnd();
		}
		
		bool Locator::pick_begin_region(double x, double tolerance)
		{
			return (rd_region.min >= (x-tolerance) && rd_region.min <= (x+tolerance));
		}

		bool Locator::pick_end_region(double x, double tolerance)
		{
			return (rd_region.max >= (x-tolerance) && rd_region.max <= (x+tolerance));
		}
	}
}

// END
