#include "CLAMGL.hxx"
#include "vm_segmentation.hxx"
#include "vm_segment_editor.hxx"

/*
#define __SEGMENT_EDITOR__DEBUG__
 */

#ifdef __SEGMENT_EDITOR__DEBUG__
#include <iostream>
#endif

namespace CLAM
{
	namespace VM
	{
		SegmentEditor::SegmentEditor()
			: rd_segmentation(0)
			, rd_mouse_pressed(false)
			, rd_allow_change_current(false)
			, rd_current_segment(0)
			, rd_edition_mode(Idle)
			, rd_dragged_segment(0)
			, rd_colors(5)
			, rd_highlighted(-1)
			, rd_vmargin(0.0)
			, rd_bound_type(LEFT_BOUND)
		{
			colorize();
		}

		SegmentEditor::~SegmentEditor()
		{
		}

		void SegmentEditor::set_segmentation(CLAM::Segmentation* s)
		{
			rd_segmentation = s;
			if(!rd_segmentation) return;
			rd_current_segment = rd_segmentation->current();
			emit requestUpdate();
		}

		void SegmentEditor::check_current(double xpos)
		{
			if(!rd_segmentation) return;
			if(!rd_enabled || !rd_allow_change_current) return;
			unsigned index = rd_segmentation->pickSegmentBody(xpos);
			if(index == rd_current_segment || index == rd_segmentation->offsets().size()) return;
			rd_segmentation->current(index);
			rd_segmentation->clearSelection();
			emit currentSegmentChanged(index);
			rd_current_segment = rd_segmentation->current();
		}

		void SegmentEditor::allow_change_current(bool allow)
		{
			rd_allow_change_current = allow;
		}

		void SegmentEditor::set_hbounds(double left, double right)
		{
			Renderer2D::set_hbounds(left,right);
		}

		void SegmentEditor::set_vbounds(double bottom, double top)
		{
			Renderer2D::set_vbounds(bottom,top);
			rd_vmargin = double(PIXMARGIN)*(rd_view.top-rd_view.bottom)/double(rd_viewport.h);
		}

		void SegmentEditor::render()
		{
			if(!rd_segmentation) return;
			if(!rd_enabled) return;
			unsigned nElems = rd_segmentation->onsets().size();
			if(!nElems) return;
			unsigned current = rd_segmentation->current();
			const Segmentation::TimePositions & beginnings = rd_segmentation->onsets();
			const Segmentation::TimePositions & endings = rd_segmentation->offsets();
			for(unsigned i=0;  i < nElems; i++)
			{
				if(beginnings[i] > rd_view.right) break;
				if(is_visible(beginnings[i],endings[i],rd_view.left,rd_view.right)) 
				{
					draw_segment(beginnings[i],
								 endings[i],
								 rd_yrange.min,
								 rd_yrange.max,
								 i==current,
								 rd_segmentation->selections()[i]);
				}				
			}
			draw_vhighlighted(rd_yrange.min,rd_yrange.max);
			if (current>=nElems) return; // no current
			if (is_visible(beginnings[current],endings[current],rd_view.left,rd_view.right)) 
				stippled_rect(beginnings[current],endings[current],rd_yrange.min,rd_yrange.max);
		}

		void SegmentEditor::mouse_move_event(double x, double y)
		{
			if(!rd_enabled) return;
			if(!rd_segmentation || !rd_catch_events) return;
			QString ttip("");
			switch (rd_edition_mode)
			{
				case DraggingOnset:
					rd_segmentation->dragOnset(rd_dragged_segment,x);
					ttip = "x:"+QString::number(rd_segmentation->onsets()[rd_dragged_segment],'f',2);
					emit toolTip(ttip);
					emit requestRefresh();	
					return;
				case DraggingOffset:
					rd_segmentation->dragOffset(rd_dragged_segment,x);
					ttip = "x:"+QString::number(rd_segmentation->offsets()[rd_dragged_segment],'f',2);
					emit toolTip(ttip);
					emit requestRefresh();	
					return;
				case DraggingBody:
					emit working(rd_key,true);
					rd_highlighted = -1;
					emit toolTip(ttip);
					return;
				default:
					// Just continue below
					rd_highlighted = -1;
					emit working(rd_key,false);
					emit toolTip(ttip);
					emit cursorChanged(QCursor(Qt::ArrowCursor));
					break;
			}

			unsigned size = rd_segmentation->onsets().size();
			double tolerance = double(TOLERANCE)*(rd_view.right-rd_view.left)/double(rd_viewport.w);

			unsigned index;
			index = rd_segmentation->pickOnset(x,tolerance);
			if(index != size) 
			{
				emit working(rd_key,true);
				rd_highlighted = index;
				rd_bound_type = LEFT_BOUND;
				ttip = "x:"+QString::number(rd_segmentation->onsets()[index],'f',2);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			index = rd_segmentation->pickOffset(x,tolerance);
			if(index != size) 
			{
				emit working(rd_key,true);
				rd_highlighted = index;
				rd_bound_type = RIGHT_BOUND;
				ttip = "x:"+QString::number(rd_segmentation->offsets()[index],'f',2);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			if(rd_keyboard.key_insert || rd_keyboard.key_ctrl)
			{
				emit working(rd_key,true);	
			}
		}

		void SegmentEditor::mouse_press_event(double x, double y)
		{
			if(!rd_enabled) return;
			if(!rd_segmentation || !rd_catch_events) return;
			if(rd_keyboard.key_insert)
			{
				unsigned index = rd_segmentation->insert(x);
				emit requestRefresh();
				emit segmentInserted(index);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Inserted segment at index  " << index << std::endl;
#endif
				return;
			}
			unsigned index;
			unsigned nSegments = rd_segmentation->onsets().size();
			double tolerance = double(TOLERANCE)*(rd_view.right-rd_view.left)/double(rd_viewport.w);
			index = rd_segmentation->pickOnset(x,tolerance);
			if(index!=nSegments)
			{
				rd_edition_mode=DraggingOnset;
				rd_dragged_segment=index;
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Dragging onset " << index << std::endl;
#endif
				return;
			}
			index = rd_segmentation->pickOffset(x,tolerance);
			if(index!=nSegments)
			{
				rd_edition_mode=DraggingOffset;
				rd_dragged_segment=index;
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Dragging offset " << index << std::endl;
#endif
				return;
			}
			index = rd_segmentation->pickSegmentBody(x);
			{
				rd_edition_mode=DraggingBody;
				rd_dragged_segment=index;
				if(rd_keyboard.key_ctrl)
				{
					(rd_segmentation->selections()[index]) 
						? rd_segmentation->deselect(index) 
						: rd_segmentation->select(index);
				}
				else
				{
					rd_segmentation->current(index);
					rd_segmentation->clearSelection();
					emit currentSegmentChanged(index);
					rd_current_segment = rd_segmentation->current();
#ifdef __SEGMENT_EDITOR__DEBUG__
					std::cout << "Current segment is " << index << std::endl;
#endif
				}
				emit requestRefresh();	
				return;
			}
		}

		void SegmentEditor::mouse_release_event(double x, double y)
		{
			if(!rd_enabled) return;
			if(!rd_segmentation || !rd_catch_events) return;
			int mode = rd_edition_mode;
			rd_edition_mode=Idle;
			rd_mouse_pressed = false;
			if(!rd_keyboard.key_ctrl) emit working(rd_key,false);
			rd_highlighted = -1;
			emit toolTip("");
			emit cursorChanged(QCursor(Qt::ArrowCursor));
			if(mode == DraggingOnset) 
			{
				emit segmentOnsetChanged(rd_dragged_segment,rd_segmentation->onsets()[rd_dragged_segment]);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Onset changed at index " << rd_dragged_segment << ", new value " 
						  << rd_segmentation->onsets()[rd_dragged_segment] << std::endl;
#endif
				return;
			}
			if(mode == DraggingOffset) 
			{
				emit segmentOffsetChanged(rd_dragged_segment,rd_segmentation->offsets()[rd_dragged_segment]);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Offset changed at index " << rd_dragged_segment << ", new value " 
						  << rd_segmentation->offsets()[rd_dragged_segment] << std::endl;
#endif
				return;
			}
		}

		void SegmentEditor::key_press_event(int key)
		{
			if(!rd_enabled) return;
			if(!rd_segmentation || !rd_catch_events) return;
			switch(key)
			{
				case Qt::Key_Insert:
				{
					emit working(rd_key,true);
					rd_keyboard.key_insert = true; 
				}
				break;
						
				case Qt::Key_Delete:
				{
					rd_keyboard.key_delete = true; 
					unsigned nSegments = rd_segmentation->onsets().size();
					unsigned index = rd_segmentation->current();
					rd_segmentation->remove(index);
					if(rd_segmentation->onsets().size() == nSegments) return; // Remove was ignored
					emit requestRefresh();
					emit segmentDeleted(index);
					emit currentSegmentChanged(rd_segmentation->current());
					rd_current_segment = rd_segmentation->current();
#ifdef __SEGMENT_EDITOR__DEBUG__
					std::cout << "Segment deleted  " << index << std::endl;
					std::cout << "Current segment is " << rd_segmentation->current() << std::endl;
#endif
				}
				break;

				case Qt::Key_Control:
				{
					emit working(rd_key,true);
					rd_keyboard.key_ctrl = true;
				}
				break;
			}
		}

		void SegmentEditor::key_release_event(int key)
		{
			if(!rd_enabled) return;
			if(!rd_segmentation || !rd_catch_events) return;
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
					emit working(rd_key,false);
					rd_keyboard.key_ctrl = false;
					break;

				default:
					break;
			}
		}

		void SegmentEditor::leave_event()
		{
			rd_highlighted = -1;
			emit toolTip("");
			emit working(rd_key,false);
		}

		void SegmentEditor::set_viewport(const CLAM::VM::GLViewport& v)
		{
			Renderer2D::set_viewport(v);
			rd_vmargin = double(PIXMARGIN)*(rd_view.top-rd_view.bottom)/double(rd_viewport.h);
		}

		void SegmentEditor::colorize()
		{
			rd_colors[NORMAL] = CLAM::VM::Color(100,200,20);
			rd_colors[CURRENT] = CLAM::VM::Color(100,200,20);
			rd_colors[SELECTED] = CLAM::VM::Color(140,240,60); 
			rd_colors[HIGHLIGHTED] = CLAM::VM::Color(255,255,0);
			rd_colors[STIPPLED] = CLAM::VM::Color(255,90,60);
		}

		void SegmentEditor::plain_rect(double left, double right, double bottom, double top)
		{
			glVertex2d(left,bottom+rd_vmargin);
			glVertex2d(left,top-rd_vmargin);
			glVertex2d(right,top-rd_vmargin);
			glVertex2d(right,bottom+rd_vmargin);
			glVertex2d(left,bottom+rd_vmargin);
		}

		void SegmentEditor::stippled_rect(double left, double right, double bottom, double top)
		{
			float vPadding = rd_vmargin-(2.0*(rd_view.top-rd_view.bottom)/double(rd_viewport.h));
			float hPadding = 3.0*(rd_view.right-rd_view.left)/double(rd_viewport.w);
			glLineWidth(2);
			// draw stippled rect
			glColor3ub(rd_colors[STIPPLED].r,rd_colors[STIPPLED].g,rd_colors[STIPPLED].b);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(FACTOR,PATTERN);
			glBegin(GL_LINE_STRIP);
			plain_rect(left-hPadding,right+hPadding,bottom-vPadding,top+vPadding);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			glLineWidth(1);
		}

		bool SegmentEditor::is_visible(double left, double right, double lBound, double rBound)
		{
			if(left >= lBound  && left <= rBound) return true;
			if(right >= lBound && right <= rBound) return true;
			if(left <= lBound && right >= rBound) return true;
			return false;
		}

		void SegmentEditor::draw_segment(double left, double right, 
										 double bottom, double top, 
										 bool isCurrent, 
										 bool isSelected)
		{
			int cindex = isSelected ? SELECTED : NORMAL;
			int lineWidth = isCurrent ? CLINEWIDTH : NLINEWIDTH;
			glLineWidth(lineWidth);
			// fill rec
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glColor4ub(rd_colors[cindex].r,rd_colors[cindex].g,rd_colors[cindex].b,125);
			glBegin(GL_QUADS);
			plain_rect(left,right,bottom,top);
			glEnd();
			glDisable(GL_BLEND);
			// draw plain
			glColor3ub(rd_colors[cindex].r,rd_colors[cindex].g,rd_colors[cindex].b);
			glBegin(GL_LINE_STRIP);
			plain_rect(left,right,bottom,top);
			glEnd();
			glLineWidth(1);
		}

		void SegmentEditor::draw_vhighlighted(double bottom,  double top)
		{
			if(rd_highlighted < 0 || rd_highlighted >= (int)rd_segmentation->onsets().size()) return;
			float x = 
				(rd_bound_type==LEFT_BOUND) 
				? rd_segmentation->onsets()[rd_highlighted] 
				: rd_segmentation->offsets()[rd_highlighted];
			bool isCurrent = (rd_highlighted == (int)rd_segmentation->current());
			int lineWidth = isCurrent ? CLINEWIDTH : NLINEWIDTH;
		   
			glLineWidth(lineWidth);
			// draw vertical highlighted 
			glColor3ub(rd_colors[HIGHLIGHTED].r,rd_colors[HIGHLIGHTED].g,rd_colors[HIGHLIGHTED].b);
			glBegin(GL_LINES);
			glVertex2d(x,top-rd_vmargin); 
			glVertex2d(x,bottom+rd_vmargin);
			glEnd();
			glLineWidth(1);
		}
	}
}

// END
