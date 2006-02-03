#ifndef __VMQT_SEGMENT_EDITOR_H__
#define __VMQT_SEGMENT_EDITOR_H__

#include <vector>
#include "vm_renderer2d.hxx"

namespace CLAM
{
	class Segmentation;
	
	namespace VM
	{
		class SegmentEditor : public Renderer2D
		{
			Q_OBJECT
		public:
			SegmentEditor();
			~SegmentEditor();

			void set_segmentation(Segmentation* s);
			
		signals:
			void segmentOnsetChanged(unsigned,double);
			void segmentOffsetChanged(unsigned,double);
			void segmentInserted(unsigned);
			void segmentDeleted(unsigned);
			void currentSegmentChanged(unsigned);

		public slots:
			void check_current(double);
			void allow_change_current(bool);

		protected:
			void render();
			void set_hbounds(double left, double right);
			void set_vbounds(double bottom, double top);
			void mouse_press_event(double x, double y);
			void mouse_release_event(double x, double y);
			void mouse_move_event(double x, double y);
			void key_press_event(int key);
			void key_release_event(int key);
			void leave_event();
			void set_viewport(const GLViewport& v);

		private:
			Segmentation* rd_segmentation;;
			
			bool     rd_mouse_pressed;
			bool     rd_allow_change_current;
			unsigned rd_current_segment;
		    
			enum { TOLERANCE=3, PIXMARGIN=5 };

			enum EditionMode 
			{
				Idle,
				DraggingOnset,
				DraggingOffset,
				DraggingBody,
				DraggingHeight
			};

			EditionMode rd_edition_mode;
			unsigned    rd_dragged_segment;

			enum { NORMAL=0, 
				   CURRENT=1, 
				   SELECTED=2, 
				   HIGHLIGHTED=3,
				   STIPPLED=4, 
				   NLINEWIDTH=2, 
				   CLINEWIDTH=2, 
				   FACTOR=4, 
				   PATTERN=0x5555 };

			enum BoundType { LEFT_BOUND=0, RIGHT_BOUND=1, BOTTOM_BOUND=2, TOP_BOUND=3 };

			std::vector<Color> rd_colors; 

			int       rd_highlighted;
			double    rd_vmargin;
			BoundType rd_bound_type;

			void colorize();
			void plain_rect(double left, double right, double bottom, double top);
			void stippled_rect(double left, double right, double bottom, double top);
			bool is_visible(double left, double right, double lBound, double rBound);
			void draw_segment(double left, double right, double bottom, double top, bool isCurrent, bool isSelected);
			void draw_vhighlighted(double bottom,  double top);

		};
	}
}

#endif

