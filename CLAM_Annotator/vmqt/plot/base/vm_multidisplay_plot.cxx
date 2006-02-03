#include "Assert.hxx"
#include "vm_ruler.hxx"
#include "vm_scroll_group.hxx"
#include "vm_plot2d.hxx"
#include "vm_data_types.hxx"
#include "vm_multidisplay_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		MultiDisplayPlot::MultiDisplayPlot(QWidget* parent)
			: WPlot(parent)
			, wp_master_id(-1)
		{
		}
		
		MultiDisplayPlot::~MultiDisplayPlot()
		{
		}

		void MultiDisplayPlot::init_multidisplay_plot(int master_id)
		{
			create_display();
			set_master_id(master_id);

			CLAM_ASSERT(wp_display.size()>0,
						"MultiDisplayPlot::init_multidisplay_plot: master display has not been created yet.");
			CLAM_ASSERT(wp_master_id >=0 && wp_master_id < (int)wp_display.size(), 
						"MultiDisplayPlot::init_multidisplay_plot: invalid masterId.");

			wp_xruler = new Ruler(this,CLAM::VM::eTop);
			wp_xruler->setFixedHeight(40);
			wp_hscroll = new ScrollGroup(CLAM::VM::eHorizontal,this);
			wp_hscroll->setFixedHeight(20);

			// connect xruler and horizontal scroll group
			connect(wp_display[wp_master_id],SIGNAL(xRulerRange(double,double)),
					wp_xruler,SLOT(updateRange(double,double)));

			connect(wp_hscroll,SIGNAL(zoomIn()),wp_display[wp_master_id],SLOT(hZoomIn()));
			connect(wp_hscroll,SIGNAL(zoomOut()),wp_display[wp_master_id],SLOT(hZoomOut()));
			connect(wp_hscroll,SIGNAL(scrollValueChanged(int)),wp_display[wp_master_id],SLOT(updateHScrollValue(int)));

			connect(wp_display[wp_master_id],SIGNAL(hZoomRatio(QString)),wp_hscroll,SLOT(updateZoomRatio(QString)));
			connect(wp_display[wp_master_id],SIGNAL(hScrollValue(int)),wp_hscroll,SLOT(updateScrollValue(int)));
			connect(wp_display[wp_master_id],SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));

			// synchronize horizontal scrolling
			for(unsigned i=0; i < wp_display.size(); i++)
			{
				if(i == (unsigned)wp_master_id) continue;
				connect(wp_display[wp_master_id],SIGNAL(xRulerRange(double,double)),
						wp_display[i],SLOT(setHBounds(double,double)));
			}
		}

		void MultiDisplayPlot::set_master_id(int id)
		{
			CLAM_ASSERT(wp_display.size()>0,"MultiDisplayPlot::set_master_id: master display has not been created yet.");
			CLAM_ASSERT(id >=0 && id < (int)wp_display.size(), "MultiDisplayPlot::set_master_id: invalid masterId.");
			wp_master_id = id;
		}

		void MultiDisplayPlot::setMaxHScroll(int value)
		{
			int max = value-wp_display[0]->width();
			if(max < 0) max = 0;
			if(wp_hscroll->get_max_scroll_value() == max) return;
			wp_hscroll->setMaxScrollValue(max);
		}
	}
}

// END
