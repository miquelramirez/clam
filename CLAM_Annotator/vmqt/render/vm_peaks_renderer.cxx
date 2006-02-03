#include "CLAMGL.hxx"
#include "vm_peaks_renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		PeaksRenderer::PeaksRenderer()
			: rd_cpoint(255,0,0)
			, rd_cline(0,255,255)
		{
		}

		PeaksRenderer::~PeaksRenderer()
		{
		}

		void PeaksRenderer::set_peaks(const Array<Peak>& peaks)
		{
			rd_cached_peaks = peaks;
			requestRefresh();
		}

		void PeaksRenderer::set_peaks_color(const Color& cpoint, const Color& cline)
		{
			rd_cpoint = cpoint;
			rd_cline = cline;
			requestRefresh();
		}

		void PeaksRenderer::render()
		{
			if(!rd_enabled || !rd_cached_peaks.Size()) return;
			int nElems = rd_cached_peaks.Size();
			// draw lines
			glLineWidth(LINE_WIDTH);
			glColor3ub(rd_cline.r,rd_cline.g,rd_cline.b);
			glBegin(GL_LINES);
			for(int i=0; i < nElems; i++)
			{
				if(rd_cached_peaks[i].freq > rd_view.right) break;
				if(rd_cached_peaks[i].freq >= rd_view.left)
				{
					glVertex2d(rd_cached_peaks[i].freq,rd_view.bottom);
					glVertex2d(rd_cached_peaks[i].freq,rd_cached_peaks[i].mag);
				}
			}
			glEnd();
			// drqw points
			glPointSize(POINT_SIZE);
			glColor3ub(rd_cpoint.r,rd_cpoint.g,rd_cpoint.b);
			glBegin(GL_POINTS);
			for(int i=0; i < nElems; i++)
			{
				if(rd_cached_peaks[i].freq > rd_view.right) break;
				if(rd_cached_peaks[i].freq >= rd_view.left)
				{
					glVertex2d(rd_cached_peaks[i].freq,rd_cached_peaks[i].mag);
				}
			}
			glEnd();
			glLineWidth(1);
			glPointSize(1);
		}

		void PeaksRenderer::mouse_move_event(double x, double y)
		{
			if(!rd_enabled) return;
			if(!rd_cached_peaks.Size() || !rd_catch_events) return;
			int peak_index = pick_peak(x,y);
			if(peak_index != -1)
			{
				emit working(rd_key,true);
				QString ttip = "freq="+QString::number(rd_cached_peaks[peak_index].freq,'f',0)+"Hz "+"mag="+QString::number(rd_cached_peaks[peak_index].mag,'f',0)+"dB";
				emit toolTip(ttip);
				return;
			}
			emit working(rd_key,false);
			emit cursorChanged(QCursor(Qt::ArrowCursor));
			emit toolTip("");
		}
		
		int PeaksRenderer::pick_peak(double x, double y)
		{
			int index = -1;
			double xtolerance = double(TOLERANCE)*(rd_view.right-rd_view.left)/double(rd_viewport.w);
			double ytolerance = double(TOLERANCE)*(rd_view.top-rd_view.bottom)/double(rd_viewport.h);
			int left_index = 0;
			int right_index = rd_cached_peaks.Size()-1;
			while(left_index <= right_index)
			{
				const int current_index = (left_index+right_index)/2;
			
				if(rd_cached_peaks[current_index].freq >= (x-xtolerance) && 
				   rd_cached_peaks[current_index].freq <= (x+xtolerance))
				{
					index = current_index;
					break;
				}
				if(x < rd_cached_peaks[current_index].freq)
				{
					right_index = current_index-1;
				}
				else if(x > rd_cached_peaks[current_index].freq)
				{
					left_index = current_index+1;
				}
			}
			if(index==-1) return -1;
			if(rd_cached_peaks[index].mag >= (y-ytolerance)) return index;
			return -1;
		}
	}
}

// END
