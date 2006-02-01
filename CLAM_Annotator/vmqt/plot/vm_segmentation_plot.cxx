#include <QGridLayout>
#include "vm_ruler.hxx"
#include "vm_plot2d.hxx"
#include "vm_scroll_group.hxx"
#include "vm_segmentation.hxx"
#include "vm_segment_editor.hxx"
#include "vm_locator_renderer.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		SegmentationPlot::SegmentationPlot(QWidget* parent)
			: WPlot(parent)
		{
			init_segmentation_plot();
		}

		SegmentationPlot::~SegmentationPlot()
		{
		}

		void SegmentationPlot::set_xrange(double xmin, double xmax, CLAM::VM::EScale scale)
		{
			wp_plot->set_xrange(xmin,xmax);
			wp_xruler->set_scale(scale);
		}

		void SegmentationPlot::set_yrange(double ymin, double ymax, CLAM::VM::EScale scale)
		{
			wp_plot->set_yrange(ymin,ymax);
			wp_yruler->set_scale(scale);
			adjust_yruler_width(ymin,ymax);
		}

		void SegmentationPlot::set_segmentation(CLAM::Segmentation* s)
		{
			static_cast<CLAM::VM::SegmentEditor*>(wp_plot->get_renderer("segmentation"))->set_segmentation(s);
		}

		void SegmentationPlot::set_zoom_steps(int hsteps, int vsteps)
		{
			wp_plot->set_zoom_steps(hsteps,vsteps);
		}

		void SegmentationPlot::backgroundWhite()
		{
			setPalette(Qt::white);
			wp_xruler->set_bg_color(CLAM::VM::Color(255,255,255));
			wp_xruler->set_fg_color(CLAM::VM::Color(0,0,0));
			wp_yruler->set_bg_color(CLAM::VM::Color(255,255,255));
			wp_yruler->set_fg_color(CLAM::VM::Color(0,0,0));
			wp_hscroll->setPalette(Qt::white);
			wp_vscroll->setPalette(Qt::white);
			wp_plot->set_bg_color(CLAM::VM::Color(255,255,255));
			static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator"))->set_locator_color(CLAM::VM::Color(250,160,30));
		}

		void SegmentationPlot::backgroundBlack()
		{
			setPalette(Qt::black);
			wp_xruler->set_bg_color(CLAM::VM::Color(0,0,0));
			wp_xruler->set_fg_color(CLAM::VM::Color(255,255,255));
			wp_yruler->set_bg_color(CLAM::VM::Color(0,0,0));
			wp_yruler->set_fg_color(CLAM::VM::Color(255,255,255));
			wp_hscroll->setPalette(Qt::darkGreen);
			wp_vscroll->setPalette(Qt::darkGreen);
			wp_plot->set_bg_color(CLAM::VM::Color(0,0,0));
			static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator"))->set_locator_color(CLAM::VM::Color(255,0,0));
		}

		void SegmentationPlot::updateLocator(double value)
		{
			static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator"))->updateLocator(value);
		}

		void SegmentationPlot::updateLocator(double value, bool flag)
		{
			static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator"))->updateLocator(value,flag);
		}

		void SegmentationPlot::setMaxVScroll(int value)
		{
			int max = value-wp_plot->height();
			if(max < 0) max=0;
			if(wp_vscroll->get_max_scroll_value() == max) return;		
			wp_vscroll->setMaxScrollValue(max);
		}

		void SegmentationPlot::setMaxHScroll(int value)
		{
			int max = value-wp_plot->width();
			if(max < 0) max=0;;
			if(wp_hscroll->get_max_scroll_value() == max) return;
			wp_hscroll->setMaxScrollValue(max);
		}

		void SegmentationPlot::init_segmentation_plot()
		{
			wp_xruler = new CLAM::VM::Ruler(this,CLAM::VM::eTop);
			wp_yruler = new CLAM::VM::Ruler(this,CLAM::VM::eLeft);

			QFontMetrics fm(wp_yruler->get_font());
			int yruler_width = fm.width("-0.0e+00")+12;

			wp_xruler->setFixedHeight(40);
			wp_yruler->setFixedWidth(yruler_width);

			wp_hscroll = new CLAM::VM::ScrollGroup(CLAM::VM::eHorizontal,this);
			wp_vscroll = new CLAM::VM::ScrollGroup(CLAM::VM::eVertical,this);

			wp_plot = new CLAM::VM::Plot2D(this);
			wp_plot->add_renderer("locator", new CLAM::VM::Locator());
			wp_plot->add_renderer("segmentation", new CLAM::VM::SegmentEditor());
			wp_plot->bring_to_front("locator");

			wp_layout = new QGridLayout(this);
			wp_layout->setMargin(0);
			wp_layout->setSpacing(0);
			wp_layout->addWidget(wp_xruler,0,1);
			wp_layout->addWidget(wp_yruler,1,0);
			wp_layout->addWidget(wp_plot,1,1); 
			wp_layout->addWidget(wp_vscroll,1,2);
			wp_layout->addWidget(wp_hscroll,2,1);

			backgroundWhite();

			connect(wp_plot,SIGNAL(xRulerRange(double,double)),wp_xruler,SLOT(updateRange(double,double)));
			connect(wp_plot,SIGNAL(yRulerRange(double,double)),wp_yruler,SLOT(updateRange(double,double)));
	
			connect(wp_vscroll,SIGNAL(zoomIn()),wp_plot,SLOT(vZoomIn()));
			connect(wp_vscroll,SIGNAL(zoomOut()),wp_plot,SLOT(vZoomOut()));
			connect(wp_vscroll,SIGNAL(scrollValueChanged(int)),wp_plot,SLOT(updateVScrollValue(int)));

			connect(wp_hscroll,SIGNAL(zoomIn()),wp_plot,SLOT(hZoomIn()));
			connect(wp_hscroll,SIGNAL(zoomOut()),wp_plot,SLOT(hZoomOut()));
			connect(wp_hscroll,SIGNAL(scrollValueChanged(int)),wp_plot,SLOT(updateHScrollValue(int)));
			
			connect(wp_plot,SIGNAL(vZoomRatio(QString)),wp_vscroll,SLOT(updateZoomRatio(QString)));
			connect(wp_plot,SIGNAL(vScrollValue(int)),wp_vscroll,SLOT(updateScrollValue(int)));
			connect(wp_plot,SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));

			connect(wp_plot,SIGNAL(hZoomRatio(QString)),wp_hscroll,SLOT(updateZoomRatio(QString)));
			connect(wp_plot,SIGNAL(hScrollValue(int)),wp_hscroll,SLOT(updateScrollValue(int)));
			connect(wp_plot,SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));

			CLAM::VM::Locator* locator=static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator"));
			connect(locator,SIGNAL(selectedRegion(double,double)),this,SIGNAL(selectedRegion(double,double)));
			CLAM::VM::SegmentEditor* editor=static_cast<CLAM::VM::SegmentEditor*>(wp_plot->get_renderer("segmentation"));
			connect(editor,SIGNAL(segmentOnsetChanged(unsigned,double)),
					this,SIGNAL(segmentOnsetChanged(unsigned,double)));
			connect(editor,SIGNAL(segmentOffsetChanged(unsigned,double)),
					this,SIGNAL(segmentOffsetChanged(unsigned,double)));
			connect(editor,SIGNAL(segmentInserted(unsigned)),this,SIGNAL(segmentInserted(unsigned)));
			connect(editor,SIGNAL(segmentDeleted(unsigned)),this,SIGNAL(segmentDeleted(unsigned)));
			connect(editor,SIGNAL(currentSegmentChanged(unsigned)),this,SIGNAL(currentSegmentChanged(unsigned)));
		}

		void SegmentationPlot::adjust_yruler_width(double min, double max)
		{
			if(wp_yruler->get_scale() == CLAM::VM::eLogScale) return;
			
			int length_min = QString::number(min,'f',2).length();
			int length_max = QString::number(max,'f',2).length();

			QFontMetrics fm(wp_yruler->get_font());

			int width = (length_min > length_max) 
				? fm.width(QString::number(min,'f',2)) 
				: fm.width(QString::number(max,'f',2));
			width += 12;
			if(wp_yruler->width() < width) wp_yruler->setFixedWidth(width);
		}
	}
}

// END
