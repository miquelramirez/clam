#ifndef __VMQT_LOCATOR_RENDERER_H__
#define __VMQT_LOCATOR_RENDERER_H__

#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{
		class Locator : public CLAM::VM::Renderer2D
		{
			Q_OBJECT
		public:
			Locator();
			~Locator();

			void set_locator_color(const CLAM::VM::Color& c);
			void set_region_color(const CLAM::VM::Color& c);

		signals:
			void selectedRegion(double,double);

		protected:
			void render();
			void mouse_press_event(double x, double y);
			void mouse_release_event(double x, double y);
			void mouse_move_event(double x, double y);
			void key_press_event(int key);
			void key_release_event(int key);
			void leave_event();

		private:
			bool            rd_region_enabled;
			double          rd_locator_pos;
			CLAM::VM::Range rd_region;
			CLAM::VM::Color rd_locator_color;
			CLAM::VM::Color rd_region_color;

			enum { TOLERANCE=3 };
			enum EditionMode 
			{
				Idle,
				DraggingBeginRegion,
				DraggingEndRegion
			};

			EditionMode rd_edition_mode;

			void draw_locator();
			void draw_region(int mode);
			int draw_region_mode();
			bool pick_begin_region(double x, double tolerance);
			bool pick_end_region(double x, double tolerance);
		};
	}
}

#endif

