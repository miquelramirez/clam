#include <QApplication>
#include "vm_segmentation_plot.hxx"
#include "vm_discontinuous_segmentation.hxx"


int main(int argc, char** argv)
{
	QApplication app( argc, argv );

	double xmax = 22050.0;
	CLAM::Segmentation* seg = new CLAM::DiscontinuousSegmentation(xmax);
	unsigned nSegments = 15;
	double step = xmax/double(nSegments);
	// make some divisions
	for(unsigned i=1; i < nSegments; i++)
	{
		seg->insert(double(i)*step);
	}
	seg->current(2);
	
	CLAM::VM::SegmentationPlot plot;
	plot.set_title("Discontinuous Segmentation");
	plot.set_geometry(100,50,600,300);
	plot.set_xrange(0.0,xmax);
	plot.set_yrange(-150.0,0.0);
	plot.set_zoom_steps(10,6);
	plot.set_segmentation(seg);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



