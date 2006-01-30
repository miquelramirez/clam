#include "CLAMGL.hxx"
#include "CLAM_Math.hxx"
#include "vm_grid.hxx"
#include "vm_bpf_editor.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFEditor::BPFEditor()
			: rd_glList(0)
			, rd_flags(0xff)
			, rd_current_index(0)
			, rd_rebuild_glList(false)
			, rd_edition_mode(Idle)

		{
		}

		BPFEditor::~BPFEditor()
		{
		}

		void BPFEditor::set_data(CLAM::BPF* bpf)
		{
			rd_bpf = bpf;
			rd_current_index = 0;
			rd_rebuild_glList = true;
			rd_edition_mode = Idle;
			emit requestUpdate();
		}

		void BPFEditor::set_colors(const CLAM::VM::Color& cline, const CLAM::VM::Color& chandler)
		{
			rd_line_color = cline;
			rd_handler_color = chandler;
			rd_rebuild_glList = true;
			emit requestRefresh();
		}

		void BPFEditor::set_flags(int flags)
		{
			rd_flags = flags;
		}

		void BPFEditor::render()
		{
			if(!rd_enabled) return;
			if(!rd_bpf) return;
			if(!rd_bpf->Size()) return;

			if(!rd_glList) rd_glList = get_glList();
			if(rd_rebuild_glList)
			{
				std::pair<long, long> bounds = get_bounds();
				glNewList(rd_glList,GL_COMPILE_AND_EXECUTE);
				// lines
				glLineWidth(LINE_WIDTH);
				glColor3ub(rd_line_color.r,rd_line_color.g,rd_line_color.b);
				glBegin(GL_LINE_STRIP);
				for(long i=bounds.first; i < bounds.second; i++)
				{
					glVertex2d(GLdouble(rd_bpf->GetXValue(i)),GLdouble(rd_bpf->GetValueFromIndex(i)));
				}
				glEnd();
				// points
				glPointSize(POINT_SIZE);
				glColor3ub(rd_handler_color.r,rd_handler_color.g,rd_handler_color.b);
				glBegin(GL_POINTS);
				for(long i=bounds.first; i < bounds.second; i++)
				{
					glVertex2d(GLdouble(rd_bpf->GetXValue(i)),GLdouble(rd_bpf->GetValueFromIndex(i)));
				}
				glEnd();
				if(rd_current_index < 0 || rd_current_index >= rd_bpf->Size()) return;
				glPointSize(POINT_SIZE+1);
				CLAM::VM::Color c(250,160,30);
				glColor3ub(c.r,c.g,c.b);
				glBegin(GL_POINTS);
				glVertex2d(GLdouble(rd_bpf->GetXValue(rd_current_index)),
						   GLdouble(rd_bpf->GetValueFromIndex(rd_current_index)));
				glEnd();
				glEndList();
				rd_rebuild_glList = false;
			}
			else
			{
				glCallList(rd_glList);
			}
		}

		void BPFEditor::set_hbounds(double left, double right)
		{
			Renderer2D::set_hbounds(left,right);
			rd_rebuild_glList = true;
		}

		void BPFEditor::mouse_press_event(double x, double y)
		{
			if(!rd_enabled) return;
			if(!rd_bpf || !rd_catch_events) return;
			if(rd_keyboard.key_insert)
			{
				if(!(rd_flags & CLAM::VM::eAllowInsertions)) return;
				long index = add(x,y);
				choose_current_point(index);
				emit elementAdded(index,x,y);
				return;
			}
			long index = pickPoint(x,y);
			if(index != -1)
			{
				rd_edition_mode = DraggingPoint;
				choose_current_point(index);
			}
		}

		void BPFEditor::mouse_release_event(double x, double y)
		{
			if(!rd_enabled) return;
			if(!rd_bpf || !rd_catch_events) return;
			int mode = rd_edition_mode;
			rd_edition_mode = Idle;
			emit toolTip("");
			emit cursorChanged(QCursor(Qt::ArrowCursor));
			if(mode == DraggingPoint) 
			{
				if(rd_flags & CLAM::VM::eAllowHorEdition)
				{
					emit xValueChanged(rd_current_index,rd_bpf->GetXValue(rd_current_index));
				}
				if(rd_flags & CLAM::VM::eAllowVerEdition)
				{
					emit yValueChanged(rd_current_index,rd_bpf->GetValueFromIndex(rd_current_index));
				}
			}
		}

		void BPFEditor::mouse_move_event(double x, double y)
		{
			if(!rd_enabled) return;
			if(!rd_bpf || !rd_catch_events) return;
			if(!rd_bpf->Size()) return;
		   
			switch (rd_edition_mode)
			{
				case DraggingPoint:
					update(x,y);
					emit toolTip(get_tooltip(double(rd_bpf->GetXValue(rd_current_index)),
											 double(rd_bpf->GetValueFromIndex(rd_current_index))));
					emit requestRefresh();	
					return;
				default:
					// Just continue below
					emit working(rd_key,false);
					emit toolTip("");
					emit cursorChanged(QCursor(Qt::ArrowCursor));
					break;
			}
	
			long index = pickPoint(x,y);
			if(index != -1) 
			{
				emit working(rd_key,true);
				choose_current_point(index);
				emit toolTip(get_tooltip(double(rd_bpf->GetXValue(rd_current_index)),
											 double(rd_bpf->GetValueFromIndex(rd_current_index))));
				if(rd_flags & CLAM::VM::eAllowHorEdition && rd_flags & CLAM::VM::eAllowVerEdition)
				{
					emit cursorChanged(QCursor(Qt::SizeAllCursor));
				}
				else if(rd_flags & CLAM::VM::eAllowHorEdition)
				{
					emit cursorChanged(QCursor(Qt::SizeHorCursor));
				}
				else if(rd_flags & CLAM::VM::eAllowVerEdition)
				{
					emit cursorChanged(QCursor(Qt::SizeVerCursor));
				}
				else
				{
					emit cursorChanged(QCursor(Qt::ArrowCursor));
				}
				return;
			}
		   
			if(rd_keyboard.key_insert)
			{
				emit working(rd_key,true);	
			}
				   
		}

		void BPFEditor::key_press_event(int key)
		{
			if(!rd_enabled) return;
			if(!rd_bpf || !rd_catch_events) return;
			switch(key)
			{
				case Qt::Key_Insert:
					emit working(rd_key,true);
					rd_keyboard.key_insert = true; 
					break;
				case Qt::Key_Delete:
					rd_keyboard.key_delete = true; 
					if(!(rd_flags & CLAM::VM::eAllowDeletions)) return;
					remove();
					if(rd_bpf->Size() > 1)
					{
						if(rd_current_index > 0)  choose_current_point(rd_current_index-1);
					}
					else
					{
						choose_current_point(0);
					}
					emit requestRefresh();
					break;
				case Qt::Key_Control:
					rd_keyboard.key_ctrl = true;
					break;
				case Qt::Key_Up:
					rd_keyboard.key_up = true;
					move_current_point_dy(get_ystep());
					break;
				case Qt::Key_Down:
					rd_keyboard.key_down = true;
					move_current_point_dy(-get_ystep());
					break;
				case Qt::Key_Left:
					rd_keyboard.key_left = true;
					move_current_point_dx(-get_xstep());
					break;
				case Qt::Key_Right:
					rd_keyboard.key_right = true;
					move_current_point_dx(get_xstep());
					break;
				case Qt::Key_PageUp:
					rd_keyboard.key_prior = true;
					if(rd_current_index <= 0) return;
					choose_current_point(rd_current_index-1);
					break;
				case Qt::Key_PageDown:
					rd_keyboard.key_next = true;
					if(rd_current_index >= rd_bpf->Size()-1) return;
					choose_current_point(rd_current_index+1);
					break;
				default:
					break;
			}
		}

		void BPFEditor::key_release_event(int key)
		{
			if(!rd_enabled) return;
			if(!rd_bpf || !rd_catch_events) return;
			switch(key)
			{
				case Qt::Key_Insert:
					emit working(rd_key,false);
					rd_keyboard.key_insert = false; 
					break;
				case Qt::Key_Delete:
					rd_keyboard.key_delete = false; 
					break;
				case Qt::Key_Control:
					rd_keyboard.key_ctrl = false;
					break;
				case Qt::Key_Up:
					rd_keyboard.key_up = false;
					break;
				case Qt::Key_Down:
					rd_keyboard.key_down = false;
					break;
				case Qt::Key_Left:
					rd_keyboard.key_left = false;
					break;
				case Qt::Key_Right:
					rd_keyboard.key_right = false;
					break;
				case Qt::Key_PageUp:
					rd_keyboard.key_prior = false;
					break;
				case Qt::Key_PageDown:
					rd_keyboard.key_next = false;
					break;
				default:
					break;
			}
		}

		void BPFEditor::leave_event()
		{
			emit toolTip("");
		}

		void BPFEditor::remove()
		{
			if(!rd_bpf->Size()) return;
			if(rd_current_index < 0 || rd_current_index >= rd_bpf->Size()) return;
			rd_rebuild_glList = true;
			rd_bpf->DeleteIndex(rd_current_index);
			emit elementRemoved(rd_current_index);
		}

		long BPFEditor::add(double x, double y)
		{
			rd_rebuild_glList = true;
			if(rd_grid)
			{
				if(rd_grid->show_grid())
				{
					if(rd_grid->snap_to_grid())
					{
						x = round((x-rd_xrange.min)/rd_grid->xstep())*rd_grid->xstep()+rd_xrange.min;
						y = round((y-rd_yrange.min)/rd_grid->ystep())*rd_grid->ystep()+rd_yrange.min;
					}
				}
			}
			return rd_bpf->Insert(x,y);
		}

		void BPFEditor::update(double x, double y)
		{
			rd_rebuild_glList = true;
			double threshold = double(TOLERANCE)*(rd_view.right-rd_view.left)/double(rd_viewport.w);
			if(rd_current_index != 0)
			{
				double prior_x = rd_bpf->GetXValue(rd_current_index-1);
				if(x < prior_x+threshold) x = prior_x+threshold;
			}
			if(rd_current_index != rd_bpf->Size()-1)
			{
				double next_x = rd_bpf->GetXValue(rd_current_index+1);
				if(x > next_x-threshold) x = next_x-threshold;
			}

			if(x < rd_xrange.min) x = rd_xrange.min;
			if(x > rd_xrange.max) x = rd_xrange.max;
			if(y < rd_yrange.min) y = rd_yrange.min;
			if(y > rd_yrange.max) y = rd_yrange.max;

			if(rd_grid)
			{
				if(rd_grid->show_grid())
				{
					if(rd_grid->snap_to_grid())
					{
						x = round((x-rd_xrange.min)/rd_grid->xstep())*rd_grid->xstep()+rd_xrange.min;
						y = round((y-rd_yrange.min)/rd_grid->ystep())*rd_grid->ystep()+rd_yrange.min;
					}
				}
			}

			if(rd_flags & CLAM::VM::eAllowHorEdition)
			{
				rd_bpf->SetXValue(rd_current_index,x);
			}
			if(rd_flags & CLAM::VM::eAllowVerEdition)
			{
				rd_bpf->SetValue(rd_current_index,y);
			}				
		}

		long BPFEditor::pickPoint(double x, double y)
		{
			long index = -1;
			double xtolerance = double(TOLERANCE)*(rd_view.right-rd_view.left)/double(rd_viewport.w);
			double ytolerance = double(TOLERANCE)*(rd_view.top-rd_view.bottom)/double(rd_viewport.h);
			long left_index = 0;
			long right_index = rd_bpf->Size()-1;
			while(left_index <= right_index)
			{
				const long current_index = (left_index+right_index)/2;
				
				if(double(rd_bpf->GetXValue(current_index)) >= (x-xtolerance) && 
				   double(rd_bpf->GetXValue(current_index)) <= (x+xtolerance))
				{
					index = current_index;
					break;
				}
				if(x < double(rd_bpf->GetXValue(current_index)))
				{
					right_index = current_index-1;
				}
				else if(x > double(rd_bpf->GetXValue(current_index)))
				{
					left_index = current_index+1;
				}
			}
			if(index==-1) return -1;
			if(double(rd_bpf->GetValueFromIndex(index)) >= (y-ytolerance) && 
			   double(rd_bpf->GetValueFromIndex(index)) <= (y+ytolerance))
			{
			    return index;
			}
			return -1;
		}

		long BPFEditor::get_bound(double search_value)
		{
			if(search_value <= rd_bpf->GetXValue(0)) return 0;
			if(search_value >= rd_bpf->GetXValue(rd_bpf->Size()-1)) return rd_bpf->Size()-1;

			long index=0;
			long left = 0;
			long right = rd_bpf->Size()-1;
			while(left <= right)
			{
				const long current = (left+right)/2;
				if(current == rd_bpf->Size()-1)
				{
					if(rd_bpf->GetXValue(current) <= search_value) 
					{
						index = current;
						break;
					}
					continue;
				}
				if(double(rd_bpf->GetXValue(current+1)) >= search_value && 
				   double(rd_bpf->GetXValue(current)) <= search_value)
				{
					index = current;
					break;
				}
				if(search_value < double(rd_bpf->GetXValue(current)))
				{
					right = current-1;
				}
				else if(search_value > double(rd_bpf->GetXValue(current)))
				{
					left = current+1;
				}
			}
			return index;
		}

		std::pair<long, long> BPFEditor::get_bounds()
		{
			if(!rd_bpf) return std::make_pair(0,0);
			if(!rd_bpf->Size()) return std::make_pair(0,0);
			if(rd_bpf->Size()==1) return std::make_pair(0,1);

			long left = get_bound(rd_view.left);
			long right = get_bound(rd_view.right);

			if(right < rd_bpf->Size()-1) right+=2;
			else if(right < rd_bpf->Size()) right++;

			return std::make_pair(left,right);
		}

		double BPFEditor::round(double x) 
		{
			double i=double(int(x));
			double frac=x-i;
			return (frac >= 0.5) ? i+1.0 : i;
		}

		QString BPFEditor::get_tooltip(double x, double y)
		{
			return "P("+QString::number(x,'f',2)+","+QString::number(y,'f',2)+")";
		}

		void BPFEditor::choose_current_point(long index)
		{
			if(!rd_bpf) return;
			if(!rd_enabled || !rd_bpf->Size()) return;
			rd_rebuild_glList = true;
			rd_current_index = index;
			emit requestRefresh();
		}

		void BPFEditor::move_current_point_dx(double dx)
		{
			if(!rd_bpf) return;
			if(!rd_enabled || !rd_bpf->Size()) return;
			if(!(rd_flags & CLAM::VM::eAllowHorEdition)) return;
			rd_rebuild_glList = true;
			double threshold = double(TOLERANCE)*(rd_view.right-rd_view.left)/double(rd_viewport.w);
			double x = rd_bpf->GetXValue(rd_current_index)+dx;
			if(rd_current_index != 0)
			{
				double prior_x = rd_bpf->GetXValue(rd_current_index-1);
				if(x < prior_x+threshold) x = prior_x+threshold;
			}
			if(rd_current_index != rd_bpf->Size()-1)
			{
				double next_x = rd_bpf->GetXValue(rd_current_index+1);
				if(x > next_x-threshold) x = next_x-threshold;
			}
			if(x < rd_xrange.min) x = rd_xrange.min;
			if(x > rd_xrange.max) x = rd_xrange.max;			
			if(rd_grid)
			{
				if(rd_grid->show_grid())
				{
					if(rd_grid->snap_to_grid())
					{
						x = round((x-rd_xrange.min)/rd_grid->xstep())*rd_grid->xstep()+rd_xrange.min;
					}
				}
			}
			rd_bpf->SetXValue(rd_current_index,x);
			emit requestRefresh();
			emit xValueChanged(rd_current_index,rd_bpf->GetXValue(rd_current_index));
		}

		void BPFEditor::move_current_point_dy(double dy)
		{
			if(!rd_bpf) return;
			if(!rd_enabled || !rd_bpf->Size()) return;
			rd_rebuild_glList = true;
			if(!(rd_flags & CLAM::VM::eAllowVerEdition)) return;
			double y = rd_bpf->GetValueFromIndex(rd_current_index)+dy;
			if(y < rd_yrange.min) y = rd_yrange.min;
			if(y > rd_yrange.max) y = rd_yrange.max;
			if(rd_grid)
			{
				if(rd_grid->show_grid())
				{
					if(rd_grid->snap_to_grid())
					{
						y = round((y-rd_yrange.min)/rd_grid->ystep())*rd_grid->ystep()+rd_yrange.min;
					}
				}
			}
			rd_bpf->SetValue(rd_current_index,y);
			emit requestRefresh();
			emit yValueChanged(rd_current_index,rd_bpf->GetValueFromIndex(rd_current_index));
		}

		double BPFEditor::get_xstep()
		{
			double dx;
			if(!rd_grid)
			{
				(rd_keyboard.key_ctrl) 
					? dx = (rd_view.right-rd_view.left)/double(rd_viewport.w)
					: dx = 5.0*(rd_view.right-rd_view.left)/double(rd_viewport.w);
				
			}
			else
			{
				if(rd_grid->show_grid())
				{
					if(rd_grid->snap_to_grid())
					{
						dx = rd_grid->xstep();
					}
					else
					{
						(rd_keyboard.key_ctrl) 
							? dx = (rd_view.right-rd_view.left)/double(rd_viewport.w)
							: dx = 5.0*(rd_view.right-rd_view.left)/double(rd_viewport.w);
					}
				}
				else
				{
					(rd_keyboard.key_ctrl) 
						? dx = (rd_view.right-rd_view.left)/double(rd_viewport.w)
						: dx = 5.0*(rd_view.right-rd_view.left)/double(rd_viewport.w);
				}
			}
			return dx;
		}
		
		double BPFEditor::get_ystep()
		{
			double dy;
			if(!rd_grid)
			{
				(rd_keyboard.key_ctrl)
					? dy = (rd_view.top-rd_view.bottom)/double(rd_viewport.h)
					: dy = 5.0*(rd_view.top-rd_view.bottom)/double(rd_viewport.h);

			}
			else
			{
				if(rd_grid->show_grid())
				{
					if(rd_grid->snap_to_grid())
					{
						dy = rd_grid->ystep();
					}
					else
					{
						(rd_keyboard.key_ctrl)
							? dy = (rd_view.top-rd_view.bottom)/double(rd_viewport.h)
							: dy = 5.0*(rd_view.top-rd_view.bottom)/double(rd_viewport.h);
					}
				}
				else
				{
					(rd_keyboard.key_ctrl)
						? dy = (rd_view.top-rd_view.bottom)/double(rd_viewport.h)
						: dy = 5.0*(rd_view.top-rd_view.bottom)/double(rd_viewport.h);
				}
			}
			return dy;
		}
	}
}

// END
