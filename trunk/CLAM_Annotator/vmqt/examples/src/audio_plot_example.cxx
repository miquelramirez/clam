#include <stdlib.h>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include "vmAudioPlot.hxx"
#include <CLAM/ContiguousSegmentation.hxx>
#include "audio_file_utils.hxx"

int main(int argc, char** argv)
{
	QApplication app( argc, argv );
	QString fn = QFileDialog::getOpenFileName(0,"Choose a mono audio file",".","Audio (*.wav *.ogg)");
	if(fn.isEmpty())
	{
		printf("No file was choosen.\n");
		exit(1);
	}
	CLAM::Audio audio;
	printf("loading audio...\n");
	qtvm_examples_utils::load_audio(fn.toAscii().data(),audio);
	printf("done\n");
	double xmax = audio.GetDuration()/1000.0;
	CLAM::Segmentation* seg = new CLAM::ContiguousSegmentation(xmax);
	unsigned nSegments = 15;
	double step = xmax/double(nSegments);
	// make some divisions
	for(unsigned i=1; i < nSegments; i++)
	{
		seg->insert(double(i)*step);
	}
	seg->current(2);
	
	CLAM::VM::AudioPlot plot;
	plot.SetTitle("Contiguous Segmentation");
	plot.SetGeometry(100,50,600,300);
	plot.SetData(audio);
	plot.SetSegmentation(seg);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



