#ifndef __VMQT_DATA_ARRAY_RENDERER_H__
#define __VMQT_DATA_ARRAY_RENDERER_H__

#include "Array.hxx"
#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{

		class DataArrayRenderer : public CLAM::VM::Renderer2D
		{
		public:
			DataArrayRenderer();
			~DataArrayRenderer();
	
			void set_data(const CLAM::DataArray& data);
			void set_data_color(const CLAM::VM::Color& c);

			void set_huge_mode_enabled(bool en);

		protected:
			void render();
			void set_hbounds(double left, double right);
			void set_vbounds(double bottom, double top);
			void set_viewport(const CLAM::VM::GLViewport& v);
			
		private:
			CLAM::DataArray rd_cached_data;
		    CLAM::DataArray rd_processed_data;
			CLAM::DataArray rd_min_array;
			CLAM::DataArray rd_max_array;

			bool rd_huge_mode;
			bool rd_huge_mode_enabled;
			bool rd_must_process_data;

			CLAM::VM::Color      rd_data_color;
			CLAM::VM::GLView     rd_local_view;
			
			void process_data();
			void draw_huge_mode();
			void draw_normal_mode();
		};
	}
}

#endif


