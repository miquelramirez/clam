#include <algorithm>
#include "CLAMGL.hxx"
#include "vm_data_array_renderer.hxx"

namespace CLAM
{
	namespace VM
	{

		DataArrayRenderer::DataArrayRenderer()
			: rd_cached_data(0) 
			, rd_huge_mode(false)
			, rd_huge_mode_enabled(true)
			, rd_must_process_data(false)
			, rd_data_color(0,0,255)
		{
		}

		DataArrayRenderer::~DataArrayRenderer()
		{
		}
	
		void DataArrayRenderer::set_data(const CLAM::DataArray& data)
		{
			rd_cached_data = &data;
			rd_must_process_data = true;
			emit requestUpdate();
		}

		void DataArrayRenderer::set_data_color(const CLAM::VM::Color& c)
		{
			rd_data_color = c;
			emit requestRefresh();
		}

		void DataArrayRenderer::set_huge_mode_enabled(bool en)
		{
			rd_huge_mode_enabled = en;
		}

		void DataArrayRenderer::render()
		{
			if(!rd_cached_data) return;
			if(!rd_enabled || !rd_cached_data->Size()) return;
			if(rd_must_process_data) {process_data(); rd_must_process_data=false;}
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(rd_local_view.left,rd_local_view.right,rd_local_view.bottom,rd_local_view.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glColor3ub(rd_data_color.r,rd_data_color.g,rd_data_color.b);
			glBegin(GL_LINE_STRIP);
			(rd_huge_mode) ? draw_huge_mode() : draw_normal_mode();
			glEnd();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void DataArrayRenderer::set_hbounds(double left, double right)
		{
			if(!rd_cached_data) return;
			if(!rd_cached_data->Size()) return;
		  
			rd_view.left = (left-rd_xrange.min)*double(rd_cached_data->Size())/rd_xrange.span();
			rd_view.right = (right-rd_xrange.min)*double(rd_cached_data->Size())/rd_xrange.span();
			rd_must_process_data = true;
		}

		void DataArrayRenderer::set_vbounds(double bottom, double top)
		{
			Renderer2D::set_vbounds(bottom,top);
			rd_local_view.bottom = rd_view.bottom;
			rd_local_view.top = rd_view.top;
		}

		void DataArrayRenderer::set_viewport(const CLAM::VM::GLViewport& v)
		{
			Renderer2D::set_viewport(v);
			rd_must_process_data = true;
		}

		void DataArrayRenderer::process_data()
		{
			rd_local_view.left = 0.0;
		   
			unsigned offset = (rd_view.left > 0) ? unsigned(rd_view.left) : 0;
			unsigned length = unsigned(rd_view.right-rd_view.left)+1;

			if(rd_huge_mode_enabled)
			{
				if(length/5 >= (unsigned)rd_viewport.w)
				{
					rd_huge_mode = true;
					unsigned start_search, end_search, search_interval_len, search_rem_interval_len;
					unsigned max_size = (unsigned)rd_viewport.w;
					rd_max_array.Resize(max_size);
					rd_min_array.Resize(max_size);
					rd_max_array.SetSize(max_size);
					rd_min_array.SetSize(max_size);

					search_interval_len = length / max_size;
					search_rem_interval_len = length % max_size;
			
					unsigned first_pass_iterations = (search_rem_interval_len) ? max_size-1 : max_size;
					start_search = offset;
					end_search = start_search + search_interval_len;				
			
					for(unsigned i=0; i < first_pass_iterations; i++)
					{
						rd_max_array[i] = *std::max_element(rd_cached_data->GetPtr()+start_search, 
														rd_cached_data->GetPtr()+end_search);
						rd_min_array[i] = *std::min_element(rd_cached_data->GetPtr()+start_search, 
														rd_cached_data->GetPtr()+end_search);
						start_search = end_search;
						end_search += search_interval_len;	
					}
					if(!search_rem_interval_len) return;
					rd_max_array[max_size-1] = *std::max_element(rd_cached_data->GetPtr()+start_search, 
															 rd_cached_data->GetPtr()+start_search+search_rem_interval_len);
					rd_min_array[max_size-1] = *std::min_element(rd_cached_data->GetPtr()+start_search, 
															 rd_cached_data->GetPtr()+start_search+search_rem_interval_len);
					rd_local_view.right = double(rd_max_array.Size())-1;
					return;
				}
			}
	
			rd_huge_mode = false;

			rd_processed_data.Resize(length+1);
			rd_processed_data.SetSize(length+1);
			std::copy(rd_cached_data->GetPtr()+offset, 
					  rd_cached_data->GetPtr()+offset+length+1, 
					  rd_processed_data.GetPtr());
			rd_local_view.right = double(rd_processed_data.Size())-1;
		}

		void DataArrayRenderer::draw_huge_mode()
		{
			for(TSize i=0; i < rd_max_array.Size(); i++)
			{
				glVertex2d(double(i),rd_max_array[i]);
				glVertex2d(double(i),rd_min_array[i]);
			}
		}

		void DataArrayRenderer::draw_normal_mode()
		{
			for(TSize i=0; i < rd_processed_data.Size(); i++)
			{
				glVertex2d(double(i),rd_processed_data[i]);
			}
		}
	}
}

// END
