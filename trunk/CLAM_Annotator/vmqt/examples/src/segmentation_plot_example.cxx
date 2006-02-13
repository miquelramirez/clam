#include <QApplication>
#include "vmSegmentationPlot.hxx"
#include "vmDiscontinuousSegmentation.hxx"


int main(int argc, char** argv)
{
	double xmax = 22050.0;
	CLAM::Segmentation* seg = new CLAM::DiscontinuousSegmentation(xmax);
	unsigned nSegments = 15;
	double step = xmax/double(nSegments);
	// make some divisions
	for(unsigned i=0; i < nSegments; i++)
	{
		seg->insert(double(i)*step);
	}
	seg->current(2);

	QApplication app( argc, argv );

	CLAM::VM::SegmentationPlot plot;
	plot.SetTitle("Discontinuous Segmentation");
	plot.SetGeometry(100,50,600,300);
	plot.SetXRange(0.0,xmax);
	plot.SetYRange(-150.0,0.0);
	plot.SetZoomSteps(10,6);
	plot.SetSegmentation(seg);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



